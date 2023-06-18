using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/30/
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Hides the use of singletons vs dependency injection
    /// </summary>
    public abstract class SomeDependencyResolver<T>
    {
        public abstract T Dependency();
    }

    public class UISystemSharedDependencyResolver : SomeDependencyResolver<UISystem>
    {
        public override UISystem Dependency() => UISystem.shared;
    }

    /// <summary>
    /// Abstract component for gestures/logic to handle dragging
    /// </summary>
    public abstract class SomeDragHandler : WorldComponent, IPointerDownHandler
    {
        public enum StateType
        {
            Default,
            Drag
        }

        /// <summary>
        /// (OPTIONAL). If not null, drag the target object when this object is tapped
        /// Useful if the object with the collider has its own transform modifier which interferes with the drag
        /// or wants to be used as a drag handle for something else.
        /// </summary>
        public SomeDragHandler dragTarget;

        public SomeDependencyResolver<UISystem> uiSystemDependencyResolver = new UISystemSharedDependencyResolver();

        public UISystem UISystem => uiSystemDependencyResolver.Dependency();

        protected StateType state;

        /// <summary>
        /// Position where the user clicked to initiate the drag
        /// </summary>
        [NonSerialized]
        public WorldPosition dragStartInputPosition = new WorldPosition(Vector3.zero);

        /// <summary>
        /// Position where this object's transform was when the drag started
        /// </summary>
        [NonSerialized]
        public WorldPosition dragStartPosition = new WorldPosition(Vector3.zero);

        public virtual void OnDragStart(WorldPosition inputPosition) { }
        public abstract void OnDragUpdate(WorldPosition inputPosition);

        public bool IsDragging
        {
            get => state == StateType.Drag;
        }

        public virtual void OnDragEnd()
        {
            state = StateType.Default;
        }

        public virtual void StartDrag(WorldPosition inputPosition)
        {
            // Allow drag to be handled by another object
            if (dragTarget)
            {
                dragTarget.StartDrag(inputPosition);
                return;
            }

            state = StateType.Drag;

            dragStartPosition.Position = gameObject.transform.position;
            dragStartInputPosition.Position = inputPosition.Position;

            var uiSystem = UISystem;
            var dragModel = new DragModel(this);
            uiSystem.StartDrag(dragModel);

            OnDragStart(inputPosition);
        }

        public void OnPointerDown(PointerEventData eventData)
        {
            //Debug.Log("DragHandler2D OnPointerDown at: " + eventData.pressPosition.ToString());

            var inputScreenPosition = eventData.pointerPressRaycast.screenPosition;
            var inputWorldPosition = UISystem.Camera.ScreenToWorldPoint(inputScreenPosition);
            StartDrag(new WorldPosition(inputWorldPosition));
        }
    }
}
