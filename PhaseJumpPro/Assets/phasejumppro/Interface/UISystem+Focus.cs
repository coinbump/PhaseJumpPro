using System;
using UnityEngine;

namespace PJ
{
	public partial class UISystem
	{
        public enum FocusDirection
        {
            Forward, Back, Up, Down, Left, Right
        }

        public virtual void SetFocus(Focusable focusable)
        {
            focusable.HasFocus = true;
        }

        public void UpdateFocusFor(Focusable focusable, bool hasFocus)
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
