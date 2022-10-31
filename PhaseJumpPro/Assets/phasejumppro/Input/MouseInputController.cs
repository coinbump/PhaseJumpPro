using UnityEngine;
using UnityEngine.InputSystem;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 5/9/22
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

		public Vector3 screenPosition { get; protected set; }
		public Vector3 worldPosition { get; protected set; }
		public Ray ray { get; protected set; }

		public Mouse mouse;

		public virtual void OnUpdate(TimeSlice time)
		{
			mouse = Mouse.current;
			if (null == mouse) { return; }

			var position = mouse.position.ReadValue();

			var camera = this.inputCamera != null ? this.inputCamera : Camera.main;
			worldPosition = camera.ScreenToWorldPoint(position);
			ray = Camera.main.ScreenPointToRay(position);
		}

		public override bool IsAvailable()
		{
			return Mouse.current != null;
		}
	}
}
