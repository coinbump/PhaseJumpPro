#if FALSE
#pragma once

#include "RenderProcessor.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Orders the nodes in a depth-first traversal order
    /// Note: By default, the world sends nodes in depth first order, so don't add this unless you
    /// need it
    class DepthFirstOrderRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        DepthFirstOrderRenderProcessor() :
            Base({ .name = "Depth first order", .phases = { RenderPhase::DrawPrepare } }) {}

        // MARK: RenderProcessor

        // TODO: this is broken with new render DAG. Fix this
        void Process(RenderCameraModel& cameraModel);
    };
} // namespace PJ
#endif
