using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Attach to objects that can be dragged
    /// </summary>
    /// REQUIREMENTS:
    /// - Attach a Physics2DRaycaster to the camera
    /// - Create an EventSystem object and choose Input System UI Input Module
    /// - Add Draggable2D to objects you want to be draggable
    /// - Attach a non-trigger collider to the draggable object
    /// - Add a UISystem behavior to the scene
    public class Draggable2D : MonoBehaviour, IPointerDownHandler
    {
        /// <summary>
        /// What to do when the object is dropped
        /// </summary>
        public enum DropType
        {
            Stay,    // Stay at position where it was dropped
            SnapBack // Snap back to original position
        }

        public UISystem uiSystem;
        public DropType dropType = DropType.Stay;

        /// <summary>
        /// (OPTIONAL). If not null, drag the target object when this object is tapped
        /// Useful if the object with th ecollider has its own transform modifier which interferes with the drag
        /// or wants to be used as a drag handle for something else.
        /// </summary>
        public Draggable2D dragTarget;

        public UISystem UISystem
        {
            get
            {
                return uiSystem != null ? uiSystem : UISystem.shared;
            }
        }

        /// <summary>
        /// Position where this object's transform was when the drag started
        /// </summary>
        [NonSerialized] public Vector3 dragStartPosition = Vector3.zero;

        /// <summary>
        /// Position where the user clicked to initiate the drag
        /// </summary>
        [NonSerialized] public Vector3 dragStartInputWorldPosition = Vector3.zero;

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

        public virtual void StartDrag(Vector3 inputWorldPosition)
        {
            dragStartPosition = gameObject.transform.position;
            dragStartInputWorldPosition = inputWorldPosition;

            var uiSystem = UISystem;

            if (dragTarget)
            {
                dragTarget.StartDrag(inputWorldPosition);
                return;
            }

            var dragModel = new DragModel(this);
            uiSystem.StartDrag(dragModel);
        }

        public void OnPointerDown(PointerEventData eventData)
        {
            //Debug.Log("Draggable2D OnPointerDown at: " + eventData.pressPosition.ToString());

            var inputScreenPosition = eventData.pointerPressRaycast.screenPosition;
            var inputWorldPosition = UISystem.Camera.ScreenToWorldPoint(inputScreenPosition);
            StartDrag(inputWorldPosition);
        }
    }
}
