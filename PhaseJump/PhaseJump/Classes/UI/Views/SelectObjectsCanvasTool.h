#pragma once

#include "CanvasTool.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    class ColorRenderer;

    /// Canvas tool that draws a marquee selection rectangle while dragging
    class SelectObjectsCanvasTool : public CanvasTool {
    protected:
        /// Renderer added to the tools node on drag start, removed on drag end/cancel
        WP<ColorRenderer> marqueeRenderer;

        /// World-space drag positions. Read by the renderer's buildMesh closure each rebuild.
        Vector2 dragStartPosition;
        Vector2 dragCurrentPosition;

    public:
        using Base = CanvasTool;
        using This = SelectObjectsCanvasTool;

        SelectObjectsCanvasTool();
    };
} // namespace PJ
