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
    /// Tool that handles drag events
    /// Example: Marquee selection, paintbrush, eraser
    /// </summary>
    public class SomeDragCanvasTool2D : SomeCanvasTool2D
    {
        public ViewPosition startDragPosition;

        public virtual void OnDragStart(ToolCanvas2D canvas, ViewPosition inputPosition)
        {
            startDragPosition = inputPosition;
        }

        public virtual void OnDragUpdate(ToolCanvas2D canvas, ViewPosition inputPosition)
        {
        }

        public virtual void OnDragEnd()
        {
        }
    }
}
