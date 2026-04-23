#pragma once

#include "Base.h"
#include "DragGestureHandler2D.h"

/*
 RATING: 4 stars
 Simple types
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    class PaintCanvas;

    /// Base class for tools that attach to a paint canvas and respond to its drag gestures
    class PaintTool : public Base {
    public:
        using Base = PJ::Base;
        using This = PaintTool;

        /// Receives drag gesture updates forwarded from the canvas while this is the active tool
        DragGestureHandler2D::OnDragGestureUpdateFunc onDragGestureUpdateFunc;

        /// Canvas this tool is attached to. Set by canvas when activating/deactivating
        PaintCanvas* canvas{};

        virtual ~PaintTool() {}
    };
} // namespace PJ
