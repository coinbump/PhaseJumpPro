using System;
using UnityEngine;

namespace PJ
{
	/// <summary>
	/// Manages common UI patterns like focus
	/// </summary>
	public class UISystem : MonoBehaviour
	{
		public static UISystem shared = new();

		/// <summary>
		/// Item that has focus
		/// </summary>
		public Focusable focusedItem;

		public UISystem()
		{
		}

		public void UpdateFocusFor(Focusable focusable, bool hasFocus)
		{
			if (hasFocus)
			{
				// Take focus away from the current focusable
				if (focusedItem)
				{
					if (focusedItem == focusable) { return; }
					var oldFocusedItem = focusedItem;
					focusedItem = null;
                    oldFocusedItem.HasFocus = false;
				}

				focusedItem = focusable;
			} else if (focusedItem == focusable) {
				focusedItem = null;
            }
		}
	}
}
