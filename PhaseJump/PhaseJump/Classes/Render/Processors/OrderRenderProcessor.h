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
    class RenderModel;

    /// Orders the nodes by their axis position in world space
    class OrderRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;
        using SortFunc = std::function<bool(RenderModel const&, RenderModel const&)>;

        SortFunc sortFunc;

        OrderRenderProcessor(SortFunc sortFunc) :
            Base("Order"),
            sortFunc(sortFunc) {}

        // MARK: RenderProcessor

        void Process(RenderSystemModel& systemModel) override;
    };
} // namespace PJ
