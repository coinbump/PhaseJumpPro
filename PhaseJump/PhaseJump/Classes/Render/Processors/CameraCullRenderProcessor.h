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

        CameraCullRenderProcessor() :
            Base({ .name = "Camera cull", .phases = { RenderPhaseId::DrawPrepare } }) {}

        // MARK: RenderProcessor

        void Process(RenderCameraModel& cameraModel) override;
    };
} // namespace PJ
