#pragma once

#include "Base.h"
#include "DragGestureHandler2D.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    class ObjectCanvas;

    /// Base class for tools that attach to an canvas and respond to its drag gestures
    class CanvasTool : public Base {
    public:
        using Base = PJ::Base;
        using This = CanvasTool;

        /// Receives drag gesture updates forwarded from the canvas while this is the active tool
        DragGestureHandler2D::OnDragGestureUpdateFunc onDragGestureUpdateFunc;

        /// Canvas this tool is attached to. Set by canvas when activating/deactivating
        ObjectCanvas* canvas{};

        virtual ~CanvasTool() {}
    };
} // namespace PJ
