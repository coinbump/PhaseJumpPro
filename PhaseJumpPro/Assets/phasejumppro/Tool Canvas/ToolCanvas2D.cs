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
            public ToolCanvas2D canvas;

            public override void OnDragStart(WorldPosition inputPosition)
            {
                base.OnDragStart(inputPosition);

                canvas.OnDragStart(inputPosition);
            }

            public override void OnDragEnd()
            {
                base.OnDragEnd();

                canvas.OnDragEnd();
            }

            public override void OnDragUpdate(WorldPosition inputPosition)
            {
                canvas.OnDragUpdate(inputPosition);
            }
        }

        protected override void Awake()
        {
            base.Awake();

            var dragHandler = gameObject.AddComponent<DragHandler>();
            dragHandler.canvas = this;
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
