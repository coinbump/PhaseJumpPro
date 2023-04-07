using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ
{
    public partial class View2D : IPointerDownHandler, IPointerClickHandler, IPointerUpHandler, IPointerEnterHandler, IPointerExitHandler
    {
        public virtual void OnPointerDown(PointerEventData eventData)
        {
            var screenPressPosition = eventData.pressPosition;
            var worldPressPosition = Camera.ScreenToWorldPoint(screenPressPosition);
            var localPressPosition = transform.InverseTransformPoint(worldPressPosition);
            var viewPressPosition = WorldToViewPosition(worldPressPosition);

            // Debug.Log("View2D.OnPointerDown: " + viewPressPosition.ToString());
        }

        public virtual void OnPointerClick(PointerEventData eventData)
        {
            var screenPressPosition = eventData.pressPosition;
            var worldPressPosition = Camera.ScreenToWorldPoint(screenPressPosition);
            var localPressPosition = transform.InverseTransformPoint(worldPressPosition);
            var viewPressPosition = WorldToViewPosition(worldPressPosition);

            // Debug.Log("View2D.OnPointerClick: " + viewPressPosition.ToString());

            // Wrap UI events for portability
            SomePointerUIEvent.InputButton button = SomePointerUIEvent.InputButton.Left;
            switch (eventData.button)
            {
                case PointerEventData.InputButton.Right:
                    button = SomePointerUIEvent.InputButton.Right;
                    break;
                case PointerEventData.InputButton.Middle:
                    button = SomePointerUIEvent.InputButton.Middle;
                    break;
                default:
                    break;
            }

            OnPointerClickEvent(new PointerClickUIEvent(new LocalPosition(localPressPosition, gameObject), button));
        }

        public virtual void OnPointerUp(PointerEventData eventData)
        {
            // Debug.Log("View2D.OnPointerUp");
        }

        public virtual void OnPointerEnter(PointerEventData eventData)
        {
        }

        public virtual void OnPointerExit(PointerEventData eventData)
        {
        }

        /*
        PHASE JUMP UI EVENTS:
        Wrappers to allow for easier porting to other languages/frameworks in the future (WIP)
        */

        public virtual void OnPointerClickEvent(PointerClickUIEvent _event)
        {
        }
    }
}
