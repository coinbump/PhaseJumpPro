using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
    /// <summary>
    /// Tracks the mouse position and gives focus to the topmost focusable object the mouse is over
    /// </summary>
    public class MouseOverFocuser2D : MonoBehaviour
    {
		private MouseInputController mouseInputController;

		private void Awake()
		{
			if (null == mouseInputController)
			{
				mouseInputController = new MouseInputController();
			}
		}

		protected virtual void Start()
        {

		}

		protected virtual void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			EvtUpdate(timeSlice);
		}

		/// <summary>
		/// On update, move at the mouse (for cursors)
		/// </summary>
		protected virtual void EvtUpdate(TimeSlice time)
		{
			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

			mouseInputController.EvtUpdate(time);

			var raycastHits = Physics2D.RaycastAll(Camera.main.ScreenToWorldPoint(mouseInputController.position), Vector2.zero);
			Focusable hitFocusable = null;

			// Debug.Log("Raycast hits size:" + raycastHits.Length.ToString());

			foreach (RaycastHit2D raycastHit in raycastHits)
            {
				if (raycastHit.collider != null)
				{
					hitFocusable = raycastHit.collider.gameObject.GetComponent<Focusable>();
				}

				// Only react to the first focusable object
				if (null != hitFocusable)
                {
					break;
                }
			}

			// Set all focusable states

			var behaviors = GameObject.FindObjectsOfType(typeof(MonoBehaviour));
			foreach (MonoBehaviour behavior in behaviors)
			{
				var iterFocusable = behavior.GetComponent<Focusable>();
				if (null == iterFocusable)
				{
					continue;
				}

				iterFocusable.HasFocus = iterFocusable == hitFocusable;
			}
		}
	}
}
