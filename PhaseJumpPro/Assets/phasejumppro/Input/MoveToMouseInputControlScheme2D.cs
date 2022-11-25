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

        protected override void Awake()
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
        public override void OnUpdate(TimeSlice time)
		{
			base.OnUpdate(time);

			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

			var target = gameObject;
			if (null == target) { return; }

			var worldPosition = mouseInputController.WorldPosition;
			if (null == worldPosition) { return; }

			target.transform.position = new Vector3(worldPosition.x, worldPosition.y, target.transform.position.z);
		}

		protected override void Update()
		{
			var timeSlice = new TimeSlice(Time.deltaTime);
			OnUpdate(timeSlice);
		}
	}
}
