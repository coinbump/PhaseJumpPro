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
    public class SomePointerDevice : SomeInputDevice
    {
        public override bool IsAvailable()
        {
            return false;
        }
    }

    /// <summary>
    /// Wraps Unity Mouse for future portability to C++
    /// </summary>
    public class MouseDevice : SomePointerDevice
    {
        public Mouse mouse
        {
            get => Mouse.current;
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
