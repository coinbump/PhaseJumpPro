using System;
using System.Collections.Generic;
using PJ;
using UnityEngine;

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

        protected DragModel dragModel;
        protected WeakGameObjectList dropTargetsOver;
        protected DragState dragState = DragState.Default;

        public DragItems DraggedItems
        {
            get
            {
                if (null != dragModel)
                {
                    var list = new List<Draggable2D> { dragModel.dragged };
                    return new DragItems(list);
                }
                else
                {
                    return new DragItems();
                }
            }
        }
    
        protected virtual void OnUpdateDrag()
        {
            if (!mouseInputController.IsAvailable()) { return; }
            var didMouseButtonUp = mouseInputController.mouse.leftButton.wasReleasedThisFrame;
            var didMouseButtonDown = mouseInputController.mouse.leftButton.wasPressedThisFrame;
            var isMouseButtonDown = mouseInputController.mouse.leftButton.isPressed;

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
                            DropDragged();
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

                var dragged = dragModel.dragged;
                if (null == dragged) { return; }

                CheckDropTargets();

                var inputWorldPosition = mouseInputController.WorldPosition;
                if (null == inputWorldPosition) { return; }
                var internalOffset = dragged.dragStartPosition - dragged.dragStartInputWorldPosition;

                dragged.transform.position = inputWorldPosition + internalOffset;
            }
            else
            {
                DropDragged();
            }
        }

        protected virtual void CheckDropTargets()
        {
            //Debug.Log("Drop: Position: " + mouseInputController.position.ToString());
            var worldPosition = mouseInputController.WorldPosition;
            if (null == worldPosition) { return; }

            var raycastHits = Physics2D.RaycastAll(worldPosition, Vector2.zero);
            DropTarget hitBehavior = null;

            //Debug.Log("Drop: RaycastHits: " + raycastHits.ToString());

            var dragItems = DraggedItems;

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                if (raycastHit.collider.gameObject == dragModel.dragged.gameObject)
                {
                    continue;
                }

                if (raycastHit.collider != null)
                {
                    hitBehavior = raycastHit.collider.gameObject.GetComponent<DropTarget>();
                }

                if (null != hitBehavior && hitBehavior.CanAcceptDrag(dragItems))
                {
                    break;
                }
            }

            bool isAlreadyIn = false;
            if (null != dropTargetsOver)
            {
                foreach (WeakReference<GameObject> dropTargetReference in dropTargetsOver)
                {
                    if (dropTargetReference.TryGetTarget(out GameObject dropTargetObject))
                    {
                        var dropTarget = dropTargetObject.GetComponent<DropTarget>();
                        if (dropTarget != hitBehavior)
                        {
                            dropTarget.OnDragLeave();
                        }
                        else
                        {
                            isAlreadyIn = true;
                        }
                    }
                }
            }

            if (null == hitBehavior)
            {
                dropTargetsOver = new WeakGameObjectList();
                return;
            }
            dropTargetsOver = new WeakGameObjectList { hitBehavior.gameObject };

            if (!isAlreadyIn)
            {
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
            }

            DropDragged();
            this.dragModel = dragModel;

            dragState = lockDrag ? DragState.LockDragMouseDown : DragState.Drag;
        }

        public virtual void DropDragged()
        {
            if (null != dropTargetsOver)
            {
                dropTargetsOver.Refresh();

                foreach (WeakReference<GameObject> dropTargetReference in dropTargetsOver)
                {
                    if (dropTargetReference.TryGetTarget(out GameObject dropTargetObject))
                    {
                        dropTargetObject.GetComponent<DropTarget>().OnDragLeave();
                    }
                }
            }

            if (null == dragModel) { return; }
            dragModel.dragged.Drop();
            dragModel = null;
            dragState = DragState.Default;
        }
    }
}
