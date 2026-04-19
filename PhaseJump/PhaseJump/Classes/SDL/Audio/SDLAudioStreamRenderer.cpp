#include "SDLAudioStreamRenderer.h"
#include "GLShaderProgram.h"
#include "PolyFrameMeshBuilder.h"
#include "RenderFeature.h"
#include "RenderMaterial.h"
#include "UIPlanner.h"
#include <cmath>
#include <cstdint>
#include <SDL3/SDL_audio.h>

using namespace std;
using namespace PJ;

/// Reads one sample starting at `bytes` and returns it as a signed amplitude
/// normalized to [-1, 1], where 0 is silence.
/// Returns 0 if the format is unknown.
float ReadSignedSample(Uint8 const* bytes, SDL_AudioFormat format) {
    if (SDL_AUDIO_ISFLOAT(format)) {
        float sample = *reinterpret_cast<float const*>(bytes);

        sample = std::max(sample, -1.0f);
        sample = std::min(sample, 1.0f);
        return sample;
    }

    auto bitSize = SDL_AUDIO_BITSIZE(format);
    auto isSigned = SDL_AUDIO_ISSIGNED(format);

    switch (bitSize) {
    case 8:
        {
            if (isSigned) {
                int8_t sample = *reinterpret_cast<int8_t const*>(bytes);
                return static_cast<float>(sample) / 128.0f;
            } else {
                uint8_t sample = *bytes;
                return (static_cast<float>(sample) - 128.0f) / 128.0f;
            }
        }
    case 16:
        {
            if (isSigned) {
                int16_t sample = *reinterpret_cast<int16_t const*>(bytes);
                return static_cast<float>(sample) / 32768.0f;
            } else {
                uint16_t sample = *reinterpret_cast<uint16_t const*>(bytes);
                return (static_cast<float>(sample) - 32768.0f) / 32768.0f;
            }
        }
    case 32:
        {
            if (isSigned) {
                int32_t sample = *reinterpret_cast<int32_t const*>(bytes);
                return static_cast<float>(static_cast<double>(sample) / 2147483648.0);
            } else {
                uint32_t sample = *reinterpret_cast<uint32_t const*>(bytes);
                return static_cast<float>(
                    (static_cast<double>(sample) - 2147483648.0) / 2147483648.0
                );
            }
        }
    default:
        return 0.0f;
    }
}

SDLAudioStreamRenderer::SDLAudioStreamRenderer(Config const& config) :
    core(this, Vector3(config.worldSize.x, config.worldSize.y, 0)),
    maxSegmentsCount(config.maxSegmentsCount),
    waveformType(config.waveformType),
    streamWindow(config.streamWindow) {

    core.model.material =
        MAKE<RenderMaterial>(RenderMaterial::Config{ .shaderId = ShaderId::ColorVary });
    core.model.material->EnableFeature(RenderFeature::Blend, config.color.a != 1);
    core.model.SetColor(config.color);

    core.model.SetBuildMeshFunc([this](RendererModel const& model) -> Mesh {
        GUARDR(audioStream, {})

        auto buffer = audioStream->AudioBuffer();
        auto bufferLength = audioStream->BufferLength();
        GUARDR(buffer && bufferLength > 0, {})

        // Determine the sample format from the SDL_AudioStream if available,
        // otherwise fall back to the source AudioSpec.
        SDL_AudioFormat format = audioStream->AudioSpec().format;
        int channels = audioStream->AudioSpec().channels;

        if (auto stream = audioStream->Stream()) {
            SDL_AudioSpec srcSpec{};
            if (SDL_GetAudioStreamFormat(stream, &srcSpec, nullptr)) {
                format = srcSpec.format;
                channels = srcSpec.channels;
            }
        }

        int const bytesPerSample = SDL_AUDIO_BYTESIZE(format);
        GUARDR(bytesPerSample > 0, {})
        if (channels <= 0) {
            channels = 1;
        }

        int const bytesPerFrame = bytesPerSample * channels;
        GUARDR(bytesPerFrame > 0, {})

        Uint32 const totalFrameCount = bufferLength / bytesPerFrame;
        GUARDR(totalFrameCount > 0, {})

        // Resolve the optional stream window to a clamped [windowStart, windowEnd)
        // frame range. Without a window, render the entire buffer.
        // All accesses are bounds-checked against totalFrameCount so a window
        // outside the buffer will never read past its end.
        Uint32 windowStartFrame = 0;
        Uint32 windowEndFrame = totalFrameCount;

        if (streamWindow) {
            int const sampleRate = audioStream->AudioSpec().freq;
            GUARDR(sampleRate > 0, {})

            // Clamp negative start to 0 before converting to frames.
            float const startSeconds = std::max(streamWindow->start, 0.0f);
            float const durationSeconds = std::max(streamWindow->duration, 0.0f);

            // Use double to avoid overflow for long streams or large time values.
            double const startFrameF =
                static_cast<double>(startSeconds) * static_cast<double>(sampleRate);
            double const endFrameF = startFrameF + static_cast<double>(durationSeconds) *
                                                       static_cast<double>(sampleRate);

            double const totalF = static_cast<double>(totalFrameCount);
            double const clampedStartF = std::min(startFrameF, totalF);
            double const clampedEndF = std::min(std::max(endFrameF, clampedStartF), totalF);

            windowStartFrame = static_cast<Uint32>(clampedStartF);
            windowEndFrame = static_cast<Uint32>(clampedEndF);
        }

        GUARDR(windowEndFrame > windowStartFrame, {})
        Uint32 const windowFrameCount = windowEndFrame - windowStartFrame;

        auto segmentsCount = std::min((int32_t)windowFrameCount, maxSegmentsCount);
        GUARDR(segmentsCount > 0, {})

        auto bucketSize = windowFrameCount / segmentsCount;
        GUARDR(bucketSize > 0, {})

        Vector2 worldSize = model.WorldSize();
        float const halfHeight = worldSize.y * 0.5f;

        // Per-bucket aggregate (first channel only)
        struct Bucket {
            float min;
            float max;
            float rms;
        };

        VectorList<Bucket> buckets;
        buckets.reserve(segmentsCount);

        for (int32_t i = 0; i < segmentsCount; i++) {
            Uint32 const startFrame = windowStartFrame + i * bucketSize;
            Uint32 const endFrame = std::min(startFrame + bucketSize, windowEndFrame);
            GUARDR(endFrame > startFrame, {})

            Uint32 const count = endFrame - startFrame;

            float minVal = 1.0f;
            float maxVal = -1.0f;
            float sumSquares = 0.0f;

            for (Uint32 f = startFrame; f < endFrame; f++) {
                // Bounds-check each frame against the total buffer length
                // before reading, so an invalid window can never read past
                // the end of the audio buffer.
                size_t const byteOffset = static_cast<size_t>(f) * bytesPerFrame;
                if (byteOffset + bytesPerSample > bufferLength) {
                    break;
                }

                Uint8 const* sampleBytes = buffer + byteOffset;
                float const sample = ReadSignedSample(sampleBytes, format);

                if (sample < minVal) {
                    minVal = sample;
                }
                if (sample > maxVal) {
                    maxVal = sample;
                }
                sumSquares += sample * sample;
            }

            Bucket bucket;
            bucket.min = minVal;
            bucket.max = maxVal;
            bucket.rms = std::sqrt(sumSquares / static_cast<float>(count));
            buckets.push_back(bucket);
        }

        // Map segment index to x coordinate, guarding the single-segment case
        // where (segmentsCount - 1) would divide by zero.
        auto xFor = [&](int32_t i) -> float {
            if (segmentsCount <= 1) {
                return 0.0f;
            }
            return (float)i / (segmentsCount - 1) * worldSize.x - worldSize.x / 2.0f;
        };

        // Build a symmetric envelope polygon: walk forward along the top edge,
        // then walk back along the bottom edge, then close the loop.
        Polygon poly;

        switch (waveformType) {
        case WaveformType::RMS:
            for (int32_t i = 0; i < segmentsCount; i++) {
                poly.Add({ xFor(i), buckets[i].rms * halfHeight });
            }
            for (int32_t i = segmentsCount - 1; i >= 0; i--) {
                poly.Add({ xFor(i), -buckets[i].rms * halfHeight });
            }
            break;

        case WaveformType::MinMax:
            for (int32_t i = 0; i < segmentsCount; i++) {
                poly.Add({ xFor(i), buckets[i].max * halfHeight });
            }
            for (int32_t i = segmentsCount - 1; i >= 0; i--) {
                poly.Add({ xFor(i), buckets[i].min * halfHeight });
            }
            break;
        }

        PolyFrameMeshBuilder builder(
            { .poly = poly, .strokeWidth = 1, .polyClose = PolyClose::Closed }
        );
        return builder.BuildMesh();
    });

    Override(planUIFuncs[UIContextId::Inspector], core.MakePlanUIFunc());
}

SDLAudioStreamRenderer& SDLAudioStreamRenderer::SetAudioStream(SP<SDLAudioStream> value) {
    audioStream = value;
    core.model.SetMeshNeedsBuild();
    return *this;
}

SDLAudioStreamRenderer& SDLAudioStreamRenderer::SetWaveformType(WaveformType value) {
    GUARDR(waveformType != value, *this)

    waveformType = value;
    core.model.SetMeshNeedsBuild();
    return *this;
}

SDLAudioStreamRenderer& SDLAudioStreamRenderer::SetStreamWindow(std::optional<StreamWindow> value) {
    GUARDR(streamWindow != value, *this)

    streamWindow = value;
    core.model.SetMeshNeedsBuild();
    return *this;
}
