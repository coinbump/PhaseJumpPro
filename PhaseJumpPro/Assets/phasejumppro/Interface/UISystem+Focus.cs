using System;
using UnityEngine;

namespace PJ
{
    public partial class UISystem
    {
        public virtual void SetFocus(FocusHandler focusable)
        {
            focusable.HasFocus = true;
        }

        /// <summary>
        /// On update, check the mouse position and set focus to the first raycast hit object with a FocusHandler component
        /// </summary>
        public virtual void OnMouseMove()
        {
            switch (mouseOverAction)
            {
                case MouseOverAction.Focus:
                    UpdateMouseOverFocus();
                    break;
                case MouseOverAction.Ignore:
                    break;
            }

            UpdateMouseOverHover();
        }

        public virtual T MouseOverHitComponent<T>(Utils.RaycastType raycastType) where T : WorldComponent
        {
            if (null == mouseDevice || !mouseDevice.IsAvailable()) { return null; }

            var screenPosition = mouseDevice.ScreenPosition;
            if (null == screenPosition) { return null; }

            //Debug.Log("Screen position:" + mouseDevice.ScreenPosition.ToString());

            var raycastHits = Utils.RaycastHits2D(Camera, new ScreenPosition(screenPosition), raycastType);
            T hitComponent = null;

            //Debug.Log("Raycast hits size:" + raycastHits.Length.ToString());

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                if (raycastHit.collider != null)
                {
                    hitComponent = raycastHit.collider.gameObject.GetComponent<T>();

                    if (hitComponent)
                    {
                        return hitComponent;
                    }
                }
            }

            return null;
        }

        public virtual void UpdateMouseOverHover()
        {
            var hitComponent = MouseOverHitComponent<SomeHoverGestureHandler>(Utils.RaycastType.First);
            ActiveHover = hitComponent != null ? hitComponent.gameObject : null;
        }

        public virtual void UpdateMouseOverFocus()
        {
            var hitFocusable = MouseOverHitComponent<FocusHandler>(Utils.RaycastType.All);

            // Only react to the first focusable object
            if (hitFocusable)
            {
                hitFocusable.HasFocus = true;
            }
        }

        public void UpdateFocusFor(FocusHandler focusable, bool hasFocus)
        {
            var focusedItem = ActiveFocus;

            if (hasFocus)
            {
                // Take focus away from the current focusable
                if (focusedItem)
                {
                    if (focusedItem == focusable) { return; }
                    var oldFocusedItem = focusedItem;
                    ActiveFocus = null;
                    oldFocusedItem.HasFocus = false;
                }

                ActiveFocus = focusable;
            }
            else if (focusedItem == focusable)
            {
                ActiveFocus = null;
            }
        }

        public void RemoveFocus()
        {
            var focusedItem = ActiveFocus;
            if (focusedItem)
            {
                focusedItem.HasFocus = false;
            }
        }
    }
}
