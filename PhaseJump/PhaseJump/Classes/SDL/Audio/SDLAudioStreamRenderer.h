#pragma once

#include "Renderer.h"
#include "SDLAudioStream.h"
#include "StreamWindow.h"
#include <optional>

namespace PJ {
    /// How per-bucket samples are aggregated into the rendered waveform
    enum class WaveformType {
        /// Root-mean-square — symmetric amplitude envelope
        RMS,

        /// Per-bucket min and max — preserves transient peaks
        MinMax
    };

    /// Renders an audio stream waveform using PolyFrameMeshBuilder
    class SDLAudioStreamRenderer : public Renderer {
    public:
        using Base = Renderer;
        using This = SDLAudioStreamRenderer;

        MaterialRendererCore core;

        struct Config {
            Vector2 worldSize;
            int32_t maxSegmentsCount = 200;
            Color color = Color::black;
            WaveformType waveformType = WaveformType::MinMax;
            std::optional<StreamWindow> streamWindow;
        };

    protected:
        SP<SDLAudioStream> audioStream;
        int32_t maxSegmentsCount = 200;
        WaveformType waveformType = WaveformType::MinMax;
        std::optional<StreamWindow> streamWindow;

    public:
        SDLAudioStreamRenderer(Config const& config);

        This& SetAudioStream(SP<SDLAudioStream> value);

        This& SetWaveformType(WaveformType value);

        WaveformType GetWaveformType() const {
            return waveformType;
        }

        This& SetStreamWindow(std::optional<StreamWindow> value);

        std::optional<StreamWindow> GetStreamWindow() const {
            return streamWindow;
        }

        // MARK: Renderer

        VectorList<RenderModel> RenderModels() override {
            return core.RenderModels();
        }

        Color GetColor() const override {
            return core.GetColor();
        }

        void SetColor(Color color) override {
            core.SetColor(color);
        }

        // MARK: WorldSizeable

        Vector3 WorldSize() const override {
            return core.model.WorldSize();
        }

        void SetWorldSize(Vector3 value) override {
            core.model.SetWorldSize(value);
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SDLAudioStreamRenderer";
        }
    };
} // namespace PJ
