using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Attach to objects that can be dragged
    /// </summary>
    /// REQUIREMENTS:
    /// - Attach a Physics2DRaycaster to the camera
    /// - Create an EventSystem object and choose Input System UI Input Module
    /// - Add DragHandler2D to objects you want to be draggable
    /// - Attach a non-trigger collider to the draggable object
    /// - Add a UISystem behavior to the scene
    public class DragHandler2D : SomeDragHandler
    {
        /// <summary>
        /// What to do when the object is dropped
        /// </summary>
        public enum DropType
        {
            Stay,    // Stay at position where it was dropped
            SnapBack // Snap back to original position
        }

        public DropType dropType = DropType.Stay;

        public virtual void Drop()
        {
            switch (dropType)
            {
                case DropType.Stay:
                    break;
                case DropType.SnapBack:
                    transform.position = dragStartPosition.Position;
                    break;
            }
        }

        public override void OnDragUpdate(WorldPosition inputPosition)
        {
            var internalOffset = dragStartPosition.Position - dragStartInputPosition.Position;
            var newPosition = inputPosition.Position + internalOffset;
            transform.position = new Vector3(newPosition.x, newPosition.y, transform.position.z);

            Debug.Log("Internal Offset: " + internalOffset.ToString() + " transform.position: " + transform.position.ToString());
        }

        public override void OnDragEnd()
        {
            base.OnDragEnd();

            Drop();
        }
    }
}
