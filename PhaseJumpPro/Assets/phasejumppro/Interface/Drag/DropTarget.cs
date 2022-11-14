using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Target that can accept drag items
    /// Requres that the object have a collider attached for raycast checks
    /// </summary>
    public class DropTarget : PJ.MonoBehaviour
    {
        public enum StateType
        {
            NotOver, DragOverAccept, DragOverReject
        }

        protected StateType state = StateType.NotOver;

        public StateType State
        {
            get => state;
            set
            {
                if (state == value) { return; }
                state = value;
                OnStateChange();
            }
        }

        protected override void Awake()
        {
#if DEBUG
            if (null == GetComponent<Collider>() && null == GetComponent<Collider2D>())
            {
                Debug.Log("Error. DropTarget requires a collider.");
            }
#endif
        }

        public virtual bool CanAcceptDrag(DragItems items)
        {
            //Debug.Log("CanAcceptDrag");
            return true;
        }

        public virtual void OnDragEnter(DragItems items)
        {
            Debug.Log("OnDragEnter");
            State = CanAcceptDrag(items) ? StateType.DragOverAccept : StateType.DragOverReject;
        }

        public virtual void OnDragLeave()
        {
            Debug.Log("OnDragLeave");
            State = StateType.NotOver;
        }

        protected void UpdateDropTargetEffect()
        {
            var dropTargetEffect = GetComponent<SomeDropTargetEffect>();
            if (dropTargetEffect)
            {
                dropTargetEffect.State = state;
            }
        }

        protected virtual void OnStateChange()
        {
            UpdateDropTargetEffect();
        }
    }
}
