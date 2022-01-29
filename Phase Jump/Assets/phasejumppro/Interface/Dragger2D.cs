using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Handles drag behavior for objects with the Draggable behavior attached
    /// </summary>
    public class Dragger2D : MonoBehaviour
    {
        protected enum State
        {
            Default, Drag, LockDragMouseDown, LockDragMouseUp
        }

        public static Dragger2D sharedInstance = null;

        /// <summary>
        /// If true, a click will start the drag, and a click will end it
        /// </summary>
        public bool lockDrag = true;

        protected MouseInputController mouseInputController;
        protected DragModel dragModel;
        protected List<DropTarget> dropTargetsOver;
        protected State state = State.Default;

        protected virtual void Awake()
        {
            if (null == mouseInputController)
            {
                mouseInputController = new MouseInputController();
            }
      
            if (null != sharedInstance)
            {
                Debug.Log("WARNING. Only 1 Dragger2D should exist.");
                return;
            }

            sharedInstance = this;
        }

        protected virtual void Update()
        {
            var didMouseButtonUp = Input.GetMouseButtonUp(0);
            var didMouseButtonDown = Input.GetMouseButtonDown(0);
            var isMouseButtonDown = Input.GetMouseButton(0);
            if (didMouseButtonDown)
            {
                Debug.Log("isMouseButtonDown: " + isMouseButtonDown.ToString() + "didMouseButtonDown: " + didMouseButtonDown.ToString());
            }

            if (!mouseInputController.IsAvailable()) { return; }
            mouseInputController.EvtUpdate(new TimeSlice(Time.deltaTime));

            if (lockDrag) {
                switch (state)
                {
                    case State.LockDragMouseDown:
                        if (didMouseButtonUp)
                        {
                            state = State.LockDragMouseUp;
                        }
                        break;
                    case State.LockDragMouseUp:
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

            if (isMouseButtonDown || state == State.LockDragMouseUp)
            {
                if (null == dragModel) { return; }

                var dragged = dragModel.dragged;
                if (null == dragged) { return; }

                CheckDropTargets();

                var inputWorldPosition = Camera.main.ScreenToWorldPoint(mouseInputController.position);
                var internalOffset = dragged.dragStartPosition - dragged.dragStartInputWorldPosition;
                
                dragged.transform.position = inputWorldPosition + internalOffset;
            }
            else
            {
                DropDragged();
            }
        }

        public DragItems DraggedItems
        {
            get
            {
                if (dragModel!= null)
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

        protected virtual void CheckDropTargets()
        {
            //Debug.Log("Drop: Position: " + mouseInputController.position.ToString());
            var raycastHits = Utils.Raycast2DHitsAtScreenPosition(mouseInputController.position);
            DropTarget hitBehavior = null;

            //Debug.Log("Drop: RaycastHits: " + raycastHits.ToString());

            var dragItems = DraggedItems;

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
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
                foreach (DropTarget dropTarget in dropTargetsOver)
                {
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

            if (null == hitBehavior)
            {
                dropTargetsOver = new List<DropTarget>();
                return;
            }
            dropTargetsOver = new List<DropTarget> { hitBehavior };

            if (!isAlreadyIn)
            {
                hitBehavior.OnDragEnter(dragItems);
            }
        }

        public virtual void StartDrag(DragModel dragModel)
        {
            // A mouse button down will start a drag but we don't want that if the drag is locked
            switch (state)
            {
                case State.LockDragMouseUp:
                    Debug.Log("Drag is locked. Cancel the current drag first.");
                    return;
            }

            DropDragged();
            this.dragModel = dragModel;

            state = lockDrag ? State.LockDragMouseDown : State.Drag;
        }

        public virtual void DropDragged()
        {
            if (null != dropTargetsOver)
            {
                foreach (DropTarget dropTarget in dropTargetsOver)
                {
                    dropTarget.OnDragLeave();
                }
            }

            if (null == dragModel) { return; }
            dragModel.dragged.Drop();
            dragModel = null;
            state = State.Default;
        }
    }
}
