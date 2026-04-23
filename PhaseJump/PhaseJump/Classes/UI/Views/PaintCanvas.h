#pragma once

#include "Color.h"
#include "Vector2.h"
#include "WorldComponent.h"

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 4/21/26
 */
namespace PJ {
    class DragGestureHandler2D;
    class PaintTool;
    class Texture;

    /// Canvas for pixel painting. Accumulates brush renders into a viewport that never clears,
    /// so each tool stroke is composited on top of the previous frame.
    class PaintCanvas : public WorldComponent {
    public:
        using Base = WorldComponent;
        using This = PaintCanvas;

        struct Config {
            /// Pixel dimensions of the canvas buffer
            Vector2 pixelSize;
        };

        /// Color paint tools use for new strokes. The brush texture is a white mask; tools
        /// tint their sprite with this color so the stamp appears in the chosen color.
        Color brushColor = Color::black;

        PaintCanvas(Config const& config);
        ~PaintCanvas() override;

        Vector2 PixelSize() const {
            return pixelSize;
        }

        WorldNode* BrushViewportNode() const {
            return brushViewportNode.lock().get();
        }

        WorldNode* CanvasViewportNode() const {
            return canvasViewportNode.lock().get();
        }

        WorldNode* CanvasRenderNode() const {
            return canvasRenderNode.lock().get();
        }

        WorldNode* ToolsNode() const {
            return toolsNode.lock().get();
        }

        SP<PaintTool> ActiveTool() const {
            return activeTool;
        }

        /// Sets the active tool. Clears `canvas` on the previous tool and points it at this.
        void SetActiveTool(SP<PaintTool> value);

        /// Replaces the canvas viewport's backing texture with `texture` (e.g. a loaded image),
        /// resizes the canvas render sprite to match, and updates `pixelSize`. Subsequent brush
        /// strokes paint on top of the swapped-in texture.
        void SwapCanvas(SP<Texture> texture);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "PaintCanvas";
        }

        void Awake() override;

    protected:
        Vector2 pixelSize{};

        WP<WorldNode> brushViewportNode;
        WP<WorldNode> canvasViewportNode;
        WP<WorldNode> canvasRenderNode;
        WP<WorldNode> toolsNode;

        SP<PaintTool> activeTool;
        SP<DragGestureHandler2D> dragHandler;
    };
} // namespace PJ
