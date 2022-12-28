using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/25/22
*/
namespace PJ
{
    /// <summary>
    /// Updates with deltas of the drag while it is in progress
    /// </summary>
    public abstract class SomeDragGestureHandler2D : SomeDragHandler
    {
        public override void OnDragUpdate(WorldPosition inputPosition)
        {
            var delta = inputPosition.Position - dragStartInputPosition.Position;
            OnDragGestureUpdate(new Vector2(delta.x, delta.y));
        }

        public virtual void OnDragGestureUpdate(Vector2 delta)
        {
        }

        public override void OnDragEnd()
        {
            OnDragGestureUpdate(Vector2.zero);
        }
    }
}
