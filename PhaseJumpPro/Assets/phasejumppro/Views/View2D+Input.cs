using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PJ {
    public partial class View2D : IPointerDownHandler, IPointerClickHandler, IPointerUpHandler {
        public void OnPointerDown(PointerEventData eventData)
        {
            var screenPressPosition = eventData.pressPosition;
            var worldPressPosition = Camera.ScreenToWorldPoint(screenPressPosition);
            var localPressPosition = transform.InverseTransformPoint(worldPressPosition);
            var viewPressPosition = WorldToViewPosition(worldPressPosition);

            Debug.Log("View2D.OnPointerDown: " + viewPressPosition.ToString());
        }

        public void OnPointerClick(PointerEventData eventData)
        {
            var screenPressPosition = eventData.pressPosition;
            var worldPressPosition = Camera.ScreenToWorldPoint(screenPressPosition);
            var localPressPosition = transform.InverseTransformPoint(worldPressPosition);
            var viewPressPosition = WorldToViewPosition(worldPressPosition);

            Debug.Log("View2D.OnPointerClick: " + viewPressPosition.ToString());

            // Wrap UI events for portability
            SomePointerUIEvent.InputButton button = SomePointerUIEvent.InputButton.Left;
            switch (eventData.button) {
                case PointerEventData.InputButton.Right:
                    button = SomePointerUIEvent.InputButton.Right;
                    break;
                case PointerEventData.InputButton.Middle:
                    button = SomePointerUIEvent.InputButton.Middle;
                    break;
                default:
                    break;
            }

            OnPointerClickEvent(new PointerClickUIEvent(localPressPosition, button));
        }

        public void OnPointerUp(PointerEventData eventData)
        {
            Debug.Log("View2D.OnPointerUp");
        }

        public virtual void OnPointerClickEvent(PointerClickUIEvent _event) {
        }
    }
}
