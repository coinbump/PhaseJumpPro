#pragma once

#include "Axis.h"
#include "RenderProcessor.h"
#include <functional>

/*
 RATING: 4 stars
 Works, but frustrum culling needs testing, rethink
 CODE REVIEW: 9/2/24
 */
namespace PJ {
    class RenderModel;

    /// Orders the nodes by their axis position in world space
    class CameraCullRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        CameraCullRenderProcessor() :
            Base("Camera cull") {}

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
