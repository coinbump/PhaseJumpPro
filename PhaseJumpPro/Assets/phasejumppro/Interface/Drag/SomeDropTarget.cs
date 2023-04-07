using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/25/22
 */
namespace PJ
{
    /// <summary>
    /// Target that can accept drag items
    /// Requres that the object have a collider attached for raycast checks
    /// </summary>
    public abstract class SomeDropTarget : WorldComponent
    {
        public SomeEffect acceptDragEffect;

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
            // Debug.Log("CanAcceptDrag");
            return true;
        }

        public virtual void OnDragEnter(DragItems items)
        {
            // Debug.Log("OnDragEnter");
            State = CanAcceptDrag(items) ? StateType.DragOverAccept : StateType.DragOverReject;
        }

        public virtual void OnDragLeave()
        {
            // Debug.Log("OnDragLeave");
            State = StateType.NotOver;
        }

        public virtual void OnAcceptDrag(DragItems items)
        {
        }

        protected void UpdateAcceptDragEffect()
        {
            if (acceptDragEffect)
            {
                acceptDragEffect.IsOn = State == StateType.DragOverAccept;
            }
        }

        protected virtual void OnStateChange()
        {
            UpdateAcceptDragEffect();
        }
    }
}
