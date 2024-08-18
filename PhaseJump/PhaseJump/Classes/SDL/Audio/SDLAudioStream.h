#pragma once

#include "Playable.h"
#include "SomeAudioStream.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SDLAudioStream : public SomeAudioStream {
    protected:
        Uint8* audioBuffer = nullptr;
        Uint32 bufferLength = 0;
        SDL_AudioSpec audioSpec;
        SDL_AudioStream* stream = nullptr;

    public:
        SDLAudioStream(Uint8* audioBuffer, Uint32 bufferLength, SDL_AudioSpec audioSpec) :
            audioBuffer(audioBuffer),
            bufferLength(bufferLength),
            audioSpec(audioSpec) {}

        virtual ~SDLAudioStream() {
            if (audioBuffer) {
                SDL_free(audioBuffer);
            }

            if (stream) {
                SDL_DestroyAudioStream(stream);
            }
        }

        void Flush() {
            GUARD(stream)
            SDL_DestroyAudioStream(stream);
            stream = nullptr;
        }

        // MARK: Updatable

        void OnUpdate(TimeSlice time) override {}

        // MARK: Playable

        void Play() override {
            GUARD(audioBuffer)
            GUARD(bufferLength > 0)
            // FUTURE: const SDL_AudioSpec outputAudioSpec = { SDL_AUDIO_S16, 2, 48000 };

            // TODO: should we reset if we're already playing? or start a new instance? Or keep
            // playing?
            Flush();

            stream = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec, nullptr, nullptr
            );
            SDL_PutAudioStreamData(stream, audioBuffer, bufferLength);
            SDL_FlushAudioStream(stream);
            SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(stream));
        }

        float Duration() const override {
            return 0;
        }

        float Progress() const override {
            return 0;
        }

        void Pause() override {}

        void Stop() override {
            GUARD(stream)
            SDL_PauseAudioDevice(SDL_GetAudioStreamDevice(stream));
        }

        bool IsPlaying() const override {
            return false;
        }

        float PlayTime() const override {
            return 0;
        }

        void SetPlayTime(float time) override {}
    };
} // namespace PJ
