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
    public class AimAtMouseInputHandler2D : SomeInputHandler
    {
        protected MouseDevice mouseDevice = new MouseDevice();

        /// <summary>
        /// On update, aim at the mouse
        /// </summary>
        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (null == mouseDevice || !mouseDevice.IsAvailable()) { return; }

            if (gameObject.TryGetComponent(out GoNode2D node))
            {
                var mouseScreenPosition = mouseDevice.ScreenPosition;
                var mouseWorldPosition = Camera.main.ScreenToWorldPoint(new Vector3(mouseScreenPosition.x, mouseScreenPosition.y, 0));
                var myWorldPosition = transform.position;
                var angle = new Angle(new Vector2(mouseWorldPosition.x, mouseWorldPosition.y) - new Vector2(myWorldPosition.x, myWorldPosition.y));
                node.Rotation = angle;
            }
        }
    }
}
