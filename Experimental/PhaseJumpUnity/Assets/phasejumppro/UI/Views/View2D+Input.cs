using System;
using UnityEngine;
using UnityEngine.EventSystems;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 6/10/23
*/
namespace PJ
{
    public partial class View2D : IPointerDownHandler, IPointerClickHandler, IPointerUpHandler, IPointerEnterHandler, IPointerExitHandler
    {
        public virtual void OnPointerDown(PointerEventData eventData)
        {
            // Debug.Log("View2D.OnPointerDown: " + viewPressPosition.ToString());

            var localPressPosition = LocalPositionFor(eventData);
            var button = InputButtonModelFor(eventData);
            OnPointerDownEvent(new PointerDownUIEvent<LocalPosition>(localPressPosition, button));
        }

        public virtual void OnPointerClick(PointerEventData eventData)
        {
            // Debug.Log("View2D.OnPointerClick: " + viewPressPosition.ToString());

            var localPressPosition = LocalPositionFor(eventData);
            var button = InputButtonModelFor(eventData);
            OnPointerClickEvent(new PointerClickUIEvent<LocalPosition>(localPressPosition, button));
        }

        public virtual void OnPointerUp(PointerEventData eventData)
        {
            var button = InputButtonModelFor(eventData);
            OnPointerUpEvent(new PointerUpUIEvent(button));
        }

        public virtual void OnPointerEnter(PointerEventData eventData) { OnPointerEnterEvent(new PointerEnterUIEvent()); }
        public virtual void OnPointerExit(PointerEventData eventData) { OnPointerExitEvent(new PointerExitUIEvent()); }

        public LocalPosition LocalPositionFor(PointerEventData eventData)
        {
            var screenPressPosition = eventData.pressPosition;
            var worldPressPosition = Camera.ScreenToWorldPoint(screenPressPosition);
            var localPressPosition = transform.InverseTransformPoint(worldPressPosition);
            return new LocalPosition(localPressPosition, gameObject);
        }

        public PointerInputButtonModel InputButtonModelFor(PointerEventData eventData)
        {
            switch (eventData.button)
            {
                case PointerEventData.InputButton.Left:
                    return new(PointerInputButton.Left, "");
                case PointerEventData.InputButton.Right:
                    return new(PointerInputButton.Right, "");
                case PointerEventData.InputButton.Middle:
                    return new(PointerInputButton.Middle, "");
                default:
                    // FUTURE: support assigning ids for extended pointer input buttons
                    return new(PointerInputButton.Extended, "");
            }
        }

        /*
        Don't rely on Unity types directly, it makes porting to other languages/frameworks difficult.
        Use Phase Jump types instead.
        */

        public virtual void OnPointerDownEvent(PointerDownUIEvent<LocalPosition> _event) { }
        public virtual void OnPointerClickEvent(PointerClickUIEvent<LocalPosition> _event) { }
        public virtual void OnPointerUpEvent(PointerUpUIEvent _event) { }
        public virtual void OnPointerEnterEvent(PointerEnterUIEvent _event) { }
        public virtual void OnPointerExitEvent(PointerExitUIEvent _event) { }
    }
}
