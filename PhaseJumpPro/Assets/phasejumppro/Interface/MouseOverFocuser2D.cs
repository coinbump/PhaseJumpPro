using UnityEngine;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Tracks the mouse position and gives focus to the first focusable object the mouse is over
	/// REQUIREMENTS:
	/// - Objects must have Collider2D
	/// - Camera must have PhysicsRaycaster2D
    /// </summary>
    public class MouseOverFocuser2D : MonoBehaviour
    {
        /// <summary>
        /// Optional. Allows input system to work with overlay cameras
        /// </summary>
        public Camera inputCamera;

        public Camera InputCamera
        {
            get
            {
                return inputCamera != null ? inputCamera : Camera.main;
            }
        }

        protected MouseInputController mouseInputController = new MouseInputController();

		/// <summary>
		/// On update, check the mouse position and set focus to the first raycast hit object with a Focusable component
		/// </summary>
		public override void OnUpdate(TimeSlice time)
		{
			base.OnUpdate(time);

			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

            var screenPosition = mouseInputController.ScreenPosition;
            if (null == screenPosition) { return; }

            //Debug.Log("Screen position:" + mouseInputController.ScreenPosition.ToString());

            var raycastHits = Utils.Raycast2DHitsAtScreenPosition(InputCamera, screenPosition.value);
			Focusable hitFocusable = null;

			//Debug.Log("Raycast hits size:" + raycastHits.Length.ToString());

			foreach (RaycastHit2D raycastHit in raycastHits)
            {
				if (raycastHit.collider != null)
				{
					hitFocusable = raycastHit.collider.gameObject.GetComponent<Focusable>();
				}

				// Only react to the first focusable object
				if (hitFocusable)
                {
					hitFocusable.HasFocus = true;
                    break;
                }
			}
		}
	}
}
