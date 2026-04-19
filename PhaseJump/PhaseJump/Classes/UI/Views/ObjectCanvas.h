#pragma once

#include "View2D.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/17/26
 */
namespace PJ {
    class CanvasTool;
    class DragGestureHandler2D;

    /// View that hosts canvas tools for objects
    class ObjectCanvas : public View2D {
    protected:
        /// Child node that parents tool renderers. Stored so tools can add/remove renderers here,
        /// separate from the canvas's own renderers, enforcing render order.
        WP<WorldNode> toolsNode;

        /// Active tool receives forwarded drag gesture updates. Tools can be swapped at runtime.
        SP<CanvasTool> activeTool;

        /// Drag handler on the canvas owner; its updates are forwarded to the active tool.
        SP<DragGestureHandler2D> dragHandler;

    public:
        using Base = View2D;
        using This = ObjectCanvas;

        ObjectCanvas();
        ~ObjectCanvas() override;

        /// @return Returns the tools node, or nullptr if Awake hasn't run
        WorldNode* ToolsNode() const {
            return toolsNode.lock().get();
        }

        SP<CanvasTool> ActiveTool() const {
            return activeTool;
        }

        /// Sets the active tool. Clears `canvas` on the previous tool and points it at this.
        void SetActiveTool(SP<CanvasTool> value);

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "ObjectCanvas";
        }

        void Awake() override;
    };
} // namespace PJ
