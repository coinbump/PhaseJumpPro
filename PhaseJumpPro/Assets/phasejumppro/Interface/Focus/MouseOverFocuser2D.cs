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
    public class MouseOverFocuser2D : WorldComponent
    {
        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
        public new Camera camera;

        public Camera Camera
        {
            get
            {
                return camera != null ? camera : Camera.main;
            }
        }

        protected MouseDevice mouseDevice = new MouseDevice();

        /// <summary>
        /// On update, check the mouse position and set focus to the first raycast hit object with a FocusHandler component
        /// </summary>
        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (null == mouseDevice || !mouseDevice.IsAvailable()) { return; }

            var screenPosition = mouseDevice.ScreenPosition;
            if (null == screenPosition) { return; }

            //Debug.Log("Screen position:" + mouseDevice.ScreenPosition.ToString());

            var raycastHits = Utils.Raycast2DHitsAtScreenPosition(Camera, screenPosition);
            FocusHandler hitFocusable = null;

            if (null == raycastHits || raycastHits.Length == 0)
            {
                UISystem.shared.RemoveFocus();
                return;
            }

            //Debug.Log("Raycast hits size:" + raycastHits.Length.ToString());

            foreach (RaycastHit2D raycastHit in raycastHits)
            {
                if (raycastHit.collider != null)
                {
                    hitFocusable = raycastHit.collider.gameObject.GetComponent<FocusHandler>();
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
