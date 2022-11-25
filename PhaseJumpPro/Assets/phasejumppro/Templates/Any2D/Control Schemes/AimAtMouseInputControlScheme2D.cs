using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 11/19/22
*/
namespace PJ
{
	/// <summary>
	/// Rotates the Node2D to aim at the mouse
	/// </summary>
	public class AimAtMouseInputControlScheme2D : SomeInputControlScheme
	{
        protected MouseInputController mouseInputController = new MouseInputController();

		/// <summary>
		/// On update, aim at the mouse
		/// </summary>
		public override void OnUpdate(TimeSlice time)
		{
			base.OnUpdate(time);

			if (null == mouseInputController || !mouseInputController.IsAvailable()) { return; }

            if (gameObject.TryGetComponent(out Node2D node))
            {
				var mouseScreenPosition = mouseInputController.ScreenPosition;
				var mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(mouseScreenPosition.x, mouseScreenPosition.y, 0));
				var myWorldPosition = transform.position;
				var angle = AngleUtils.Vector2ToDegreeAngle(new Vector2(mouseWorldPosition.x, mouseWorldPosition.y) - new Vector2(myWorldPosition.x, myWorldPosition.y));
				node.Rotation = angle;
            }
        }
	}
}
