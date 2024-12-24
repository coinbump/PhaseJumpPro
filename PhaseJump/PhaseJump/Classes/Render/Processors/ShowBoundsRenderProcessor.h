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

        // TODO: re-evaluate all registered render phases, probably don't want to run for each
        // camera
        ShowBoundsRenderProcessor(Color color = Color::green) :
            Base({ .name = "Show bounds", .phases = { RenderPhase::Camera } }),
            color(color) {}

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
