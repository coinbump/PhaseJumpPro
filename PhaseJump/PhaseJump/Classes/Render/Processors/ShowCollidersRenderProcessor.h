#pragma once

#include "Color.h"
#include "PropertyWrapper.h"
#include "RenderProcessor.h"
#include "RenderTypes.h"

/*
 RATING 4 stars
 Works, but limited functionality. Not optimized
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show colliders (limited functionality)
    class ShowCollidersRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        Color color;

        ShowCollidersRenderProcessor(Color color = Color::blue) :
            Base({ .name = "Show colliders", .phases = { RenderPhaseId::DrawPrepare } }),
            color(color) {}

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
