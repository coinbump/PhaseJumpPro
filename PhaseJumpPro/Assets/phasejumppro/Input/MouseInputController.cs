using UnityEngine;
using UnityEngine.InputSystem;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/13/22
 */
namespace PJ
{
    /// <summary>
    /// Records mouse position on update
    /// </summary>
    public class MouseInputController : SomeInputController
	{
		/// <summary>
        /// By default, main camera is used, or link to overlay camera if needed
        /// </summary>
		public Camera inputCamera;

        public Mouse mouse
        {
            get => Mouse.current;
        }

        public Camera InputCamera
        {
            get
            {
                return inputCamera != null ? inputCamera : Camera.main;
            }
        }

        public Optional<Vector3> ScreenPosition
		{
			get
			{
                var mouse = Mouse.current;
                if (null == mouse) { return null; }

				var position = mouse.position.ReadValue();
				return new Optional<Vector3>(new Vector3(position.x, position.y, 0));
            }
        }

		public Optional<Vector3> WorldPosition
		{
			get
			{
				var screenPosition = ScreenPosition;
				if (null == screenPosition) { return null; }

                return new Optional<Vector3>(InputCamera.ScreenToWorldPoint(screenPosition.value));
            }
        }

        public Optional<Ray> ScreenRay
        {
            get
            {
                var screenPosition = ScreenPosition;
                if (null == screenPosition) { return null; }

                return new Optional<Ray>(Camera.main.ScreenPointToRay(screenPosition.value));
            }
        }

		public override bool IsAvailable()
		{
			return Mouse.current != null;
		}
	}
}
