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
    class MaterialRenderModel;

    /// Orders the nodes by their axis position in world space
    class CameraCullRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        CameraCullRenderProcessor() :
            Base({ .name = "Camera cull", .phases = { RenderPhaseId::DrawPrepare } }) {}

        // MARK: RenderProcessor

        // TODO: this is broken with new render DAG. Fix this
        void Process(RenderCameraModel& cameraModel);
    };
} // namespace PJ
