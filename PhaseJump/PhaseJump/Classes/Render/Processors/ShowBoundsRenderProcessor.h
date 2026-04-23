#pragma once

#include "Color.h"
#include "PropertyMirror.h"
#include "RenderProcessor.h"

/*
 RATING 4 stars
 Works, but limited functionality
 CODE REVIEW: 8/27/24
 */
namespace PJ {
    /// Renders a frame to show bounds
    class ShowBoundsRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        Color color;

        ShowBoundsRenderProcessor(Color color = Color::green) :
            Base({ .name = "Show bounds", .phases = { RenderPhaseId::DrawPrepare } }),
            color(color) {}

        // MARK: RenderProcessor

        // TODO: this is broken with new render DAG. Fix this
        void Process(RenderCameraModel& cameraModel);
    };
} // namespace PJ
