#pragma once

#include "Axis.h"
#include "RenderProcessor.h"
#include <functional>

/*
 RATING: 3 stars
 Buggy, but reviewed
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class RenderModel;

    /// Orders the nodes by their axis position in world space
    class CameraCullRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        // TODO: re-evaluate all registered render phases, probably don't want to run for each
        // camera
        CameraCullRenderProcessor() :
            Base({ .name = "Camera cull", .phases = { RenderPhase::Camera } }) {}

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
