#pragma once

#include "PaintTool.h"
#include "Vector2.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    class WorldNode;

    /// Paints brush stamps into the paint canvas by dragging a brush-textured sprite.
    class BrushPaintTool : public PaintTool {
    public:
        using Base = PaintTool;
        using This = BrushPaintTool;

        BrushPaintTool();

    protected:
        WP<WorldNode> brushSpriteNode;

        /// Creates the brush sprite child of the canvas viewport on demand. No-op if it exists.
        void MakeBrushSprite();

        /// Positions the brush sprite under the drag in canvas-viewport local space.
        void MoveBrushTo(Vector2 worldPos);
    };
} // namespace PJ
