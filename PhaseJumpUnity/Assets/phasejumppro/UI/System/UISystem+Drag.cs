using System;
using System.Collections.Generic;
using PJ;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    public partial class UISystem
    {
        protected enum DragState
        {
            Default, Drag, LockDragMouseDown, LockDragMouseUp
        }

        /// <summary>
        /// If true, a click will start the drag, and a click will end it
        /// </summary>
        public bool lockDrag = false;

        /// Model that defines a drag in progress
        protected DragModel dragModel;

        /// Object that a drag is over
        protected WeakReference<GameObject> dropTargetOverObject;

        /// State of drag in progress
        protected DragState dragState = DragState.Default;

        public bool IsDragging
        {
            get => dragState != DragState.Default;
        }

        public DragItems DraggedItems
        {
            get
            {
                if (null != dragModel)
                {
                    var list = new List<SomeDragHandler> { dragModel.dragHandler };
                    return new DragItems(list);
                }
                else
                {
                    return new DragItems();
                }
            }
        }

        protected virtual void OnDragUpdate()
        {
            if (!IsDragging) { return; }
            if (null == mouseDevice || !mouseDevice.IsAvailable()) { return; }
            var didMouseButtonUp = mouseDevice.mouse.leftButton.wasReleasedThisFrame;
            var didMouseButtonDown = mouseDevice.mouse.leftButton.wasPressedThisFrame;
            var isMouseButtonDown = mouseDevice.mouse.leftButton.isPressed;

            //if (didMouseButtonDown)
            //{
            //    Debug.Log("isMouseButtonDown: " + isMouseButtonDown.ToString() + "didMouseButtonDown: " + didMouseButtonDown.ToString());
            //}

            if (lockDrag)
            {
                switch (dragState)
                {
                    case DragState.LockDragMouseDown:
                        if (didMouseButtonUp)
                        {
                            dragState = DragState.LockDragMouseUp;
                        }
                        break;
                    case DragState.LockDragMouseUp:
                        if (didMouseButtonDown)
                        {
                            OnDragEnd();
                            return;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (isMouseButtonDown || dragState == DragState.LockDragMouseUp)
            {
                if (null == dragModel) { return; }

                var dragged = dragModel.dragHandler;
                if (null == dragged) { return; }

                CheckDropTargets();

                var inputWorldPosition = Camera.ScreenToWorldPoint(mouseDevice.ScreenPosition);
                if (null == inputWorldPosition) { return; }

                dragged.OnDragUpdate(new WorldPosition(inputWorldPosition));
            }
            else
            {
                OnDragEnd();
            }
        }

        protected virtual void CheckDropTargets()
        {
            //Debug.Log("Drop: Position: " + mouseDevice.position.ToString());
            var worldPosition = Camera.ScreenToWorldPoint(mouseDevice.ScreenPosition);
            if (null == worldPosition) { return; }

            var raycastHits = Physics2D.RaycastAll(worldPosition, Vector2.zero);
            SomeDropTarget hitBehavior = null;

            //Debug.Log("Drop: RaycastHits: " + raycastHits.ToString());

            var dragItems = DraggedItems;

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                if (raycastHit.collider.gameObject == dragModel.dragHandler.gameObject)
                {
                    continue;
                }

                if (raycastHit.collider != null)
                {
                    hitBehavior = raycastHit.collider.gameObject.GetComponent<SomeDropTarget>();
                }

                if (null != hitBehavior && hitBehavior.CanAcceptDrag(dragItems))
                {
                    break;
                }
            }

            bool isAlreadyIn = false;
            if (null != dropTargetOverObject && dropTargetOverObject.TryGetTarget(out GameObject dropTargetTarget))
            {
                var activeDropTarget = dropTargetTarget.GetComponent<SomeDropTarget>();
                if (activeDropTarget != hitBehavior)
                {
                    activeDropTarget.OnDragLeave();
                }
                else
                {
                    isAlreadyIn = true;
                }
            }

            if (null == hitBehavior)
            {
                dropTargetOverObject = null;
                return;
            }

            if (!isAlreadyIn)
            {
                dropTargetOverObject = new WeakReference<GameObject>(hitBehavior.gameObject);
                hitBehavior.OnDragEnter(dragItems);
            }
        }

        public virtual void StartDrag(DragModel dragModel)
        {
            // A mouse button down will start a drag but we don't want that if the drag is locked
            switch (dragState)
            {
                case DragState.LockDragMouseUp:
                    //Debug.Log("Drag is locked. Cancel the current drag first.");
                    return;
                default:
                    break;
            }

            OnDragEnd();
            this.dragModel = dragModel;

            dragState = lockDrag ? DragState.LockDragMouseDown : DragState.Drag;
        }

        public virtual void OnDragEnd()
        {
            if (null != dropTargetOverObject && dropTargetOverObject.TryGetTarget(out GameObject dropTargetTarget))
            {
                var dropTarget = dropTargetTarget.GetComponent<SomeDropTarget>();
                dropTarget.OnDragLeave();

                if (dropTarget.CanAcceptDrag(DraggedItems))
                {
                    dropTarget.OnAcceptDrag(DraggedItems);
                }
            }
            dropTargetOverObject = null;

            if (null != dragModel)
            {
                dragModel.dragHandler.OnDragEnd();
                dragModel = null;
            }
            dragState = DragState.Default;
        }
    }
}
