using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Standard behavior
 * CODE REVIEW: 1/28/22
 */
namespace PJ
{
    public class Draggable2D : MonoBehaviour
    {
        /// <summary>
        /// What to do when the object is dropped
        /// </summary>
        public enum DropType
        {
            Stay,   // Stay at position where it was dropped
            SnapBack    // Snap back to original position
        }

        public DropType dropType = DropType.Stay;

        [System.NonSerialized]
        public Vector3 dragStartPosition = Vector3.zero;

        [System.NonSerialized]
        public Vector3 dragStartInputWorldPosition = Vector3.zero;

        protected virtual void OnMouseDown()
        {
            var inputWorldPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            dragStartPosition = gameObject.transform.position;
            dragStartInputWorldPosition = inputWorldPosition;

            var dragger = Dragger2D.sharedInstance;
            if (null == dragger) {
                Debug.Log("WARNING. Missing Dragger2D component in scene.");
                return;
            }

            var dragModel = new DragModel(this);
            dragger.StartDrag(dragModel);
        }

        public virtual void Drop()
        {
            switch (dropType)
            {
                case DropType.Stay:
                    break;
                case DropType.SnapBack:
                    transform.position = dragStartPosition;
                    break;
            }
        }
    }
}
