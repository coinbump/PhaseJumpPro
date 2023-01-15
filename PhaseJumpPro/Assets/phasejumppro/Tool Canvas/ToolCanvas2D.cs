using System;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/14/23
*/
namespace PJ
{
    /// <summary>
    /// A specialized view that passes UI events to tools
    /// Example: brush tool, marquee selection, lasso selection, etc.
    /// </summary>
    public class ToolCanvas2D : View2D
    {
        public SomeCanvasTool2D activeTool;

        /// <summary>
        /// Handles drag events since View2D already has pointer logic
        /// </summary>
        protected class DragHandler : SomeDragHandler
        {
            public WeakReference<ToolCanvas2D> canvas;

            public override void OnDragStart(WorldPosition inputPosition)
            {
                base.OnDragStart(inputPosition);

                if (!canvas.TryGetTarget(out ToolCanvas2D toolCanvas)) { return; }
                toolCanvas.OnDragStart(inputPosition);
            }

            public override void OnDragEnd()
            {
                if (!canvas.TryGetTarget(out ToolCanvas2D toolCanvas)) { return; }
                toolCanvas.OnDragEnd();
            }

            public override void OnDragUpdate(WorldPosition inputPosition)
            {
                if (!canvas.TryGetTarget(out ToolCanvas2D toolCanvas)) { return; }
                toolCanvas.OnDragUpdate(inputPosition);
            }
        }

        protected override void Awake()
        {
            base.Awake();

            var dragHandler = gameObject.AddComponent<DragHandler>();
            dragHandler.canvas = new WeakReference<ToolCanvas2D>(this);
        }

        public virtual void OnDragStart(WorldPosition inputPosition)
        {
            var activeDragTool = activeTool as SomeDragCanvasTool2D;
            if (null == activeDragTool) { return; }

            var viewPosition = WorldToViewPosition(inputPosition.position);
            activeDragTool.OnDragStart(this, new ViewPosition(viewPosition));
        }

        public virtual void OnDragUpdate(WorldPosition inputPosition)
        {
            var activeDragTool = activeTool as SomeDragCanvasTool2D;
            if (null == activeDragTool) { return; }

            var viewPosition = WorldToViewPosition(inputPosition.position);
            activeDragTool.OnDragUpdate(this, new ViewPosition(viewPosition));
        }

        public virtual void OnDragEnd()
        {
            var activeDragTool = activeTool as SomeDragCanvasTool2D;
            if (null == activeDragTool) { return; }

            activeDragTool.OnDragEnd();
        }
    }
}
