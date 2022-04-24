using UnityEngine;
using System;

#if ENABLE_INPUT_SYSTEM
using UnityEngine.InputSystem;
#endif

/*
 * RATING: 5 stars
 * Tested with new Input System
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
	/// <summary>
	/// Records mouse position on update
	/// </summary>
	public class MouseInputController : SomeInputController
	{
		public Vector3 position { get; protected set; }
		public Vector3 worldPosition { get; protected set; }
		public Ray ray { get; protected set; }

		public virtual void OnUpdate(TimeSlice time)
		{
#if ENABLE_INPUT_SYSTEM
			Vector3 position = Vector3.zero;
			var mouse = Mouse.current;
			if (null !=  mouse)
            {
				position = mouse.position.ReadValue();
            }
#else
			position = Input.mousePosition;
#endif
			worldPosition = Camera.main.ScreenToWorldPoint(position);
			ray = Camera.main.ScreenPointToRay(position);
		}

		public override bool IsAvailable()
		{
#if ENABLE_INPUT_SYSTEM
			return Mouse.current != null;
#else
			return Input.mousePresent;
#endif
		}
	}
}
