#pragma once

#include "Axis.h"
#include "RenderProcessor.h"
#include <functional>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    class MaterialRenderModel;

    /// Orders the nodes by their axis position in world space
    class OrderRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;
        using SortFunc =
            std::function<bool(MaterialRenderModel const&, MaterialRenderModel const&)>;

        SortFunc sortFunc;

        OrderRenderProcessor(SortFunc sortFunc) :
            Base({ .name = "Order", .phases = { RenderPhaseId::DrawPrepare } }),
            sortFunc(sortFunc) {}

        // MARK: RenderProcessor

        // TODO: this is broken with new render DAG. Fix this
        void Process(RenderCameraModel& cameraModel);
    };
} // namespace PJ
