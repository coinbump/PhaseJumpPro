using UnityEngine;
using System;

/*
 * RATING: 5 stars
 * Tested with new Input System
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
	/// <summary>
	/// Moves the center of the gameObject to the mouse position
	/// </summary>
	public class MoveToMouseInputControlScheme2D : SomeInputControlScheme
	{
		private MouseInputController mouseInputController;

		private void Awake()
		{
			if (null == mouseInputController) { 
				mouseInputController = new MouseInputController();
			}
		}

		public MoveToMouseInputControlScheme2D()
		{
			mouseInputController = new MouseInputController();
		}

		/// <summary>
		/// On update, move at the mouse (for cursors)
		/// </summary>
		public virtual void OnUpdate(TimeSlice time)
		{
			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

			var target = gameObject;
			if (null == target) { return; }

			mouseInputController.OnUpdate(time);

			var worldPosition = mouseInputController.worldPosition;
			worldPosition.z = target.transform.position.z;
			target.transform.position = worldPosition;
		}

		protected virtual void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			OnUpdate(timeSlice);
		}
	}
}
