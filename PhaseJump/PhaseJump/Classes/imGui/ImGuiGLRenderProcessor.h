#pragma once

#include "RenderProcessor.h"

class ImGuiContext;

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/15/24
 */
namespace PJ {
    /// Renders imGUI frames in an SDL + OpenGL context
    class ImGuiGLRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        ImGuiContext* imGuiContext{};

        ImGuiGLRenderProcessor(ImGuiContext* imGuiContext) :
            // Unnamed, so it doesn't show up in render processor list
            Base({ .phases = { RenderPhase::PrepareBind, RenderPhase::PreparePresent } }),
            imGuiContext(imGuiContext) {}

        // MARK: RenderProcessor

        void Process(String phase) override;
    };
} // namespace PJ
