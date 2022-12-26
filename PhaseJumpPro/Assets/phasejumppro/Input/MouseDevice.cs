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
    /// Input device that points
    /// </summary>
    public class PointerDevice : SomeInputDevice
    {
        public override bool IsAvailable()
        {
            return false;
        }
    }

    /// <summary>
    /// Wraps Unity Mouse for future portability to C++
    /// </summary>
    public class MouseDevice : PointerDevice
    {
        /// <summary>
        /// Optional. Allows us to work with overlay cameras as well as the main camera
        /// </summary>
		public Camera camera;

        public Mouse mouse
        {
            get => Mouse.current;
        }

        public Camera Camera
        {
            get
            {
                return camera != null ? camera : Camera.main;
            }
        }

        public Vector3 ScreenPosition
        {
            get
            {
                var mouse = Mouse.current;
                if (null == mouse) { return Vector3.zero; }

                var position = mouse.position.ReadValue();
                return new Vector3(position.x, position.y, 0);
            }
        }

        public Vector3 WorldPosition
        {
            get
            {
                var screenPosition = ScreenPosition;
                return Camera.ScreenToWorldPoint(screenPosition);
            }
        }

        public Ray ScreenRay
        {
            get
            {
                var screenPosition = ScreenPosition;
                return Camera.main.ScreenPointToRay(screenPosition);
            }
        }

        public override bool IsAvailable()
        {
            return Mouse.current != null;
        }
    }
}
