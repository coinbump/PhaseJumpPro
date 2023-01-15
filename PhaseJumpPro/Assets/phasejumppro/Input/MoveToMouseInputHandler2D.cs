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
    public class MoveToMouseInputHandler2D : SomeInputHandler
    {
        private MouseDevice mouseDevice;

        protected override void Awake()
        {
            if (null == mouseDevice)
            {
                mouseDevice = new MouseDevice();
            }
        }

        public MoveToMouseInputHandler2D()
        {
            mouseDevice = new MouseDevice();
        }

        /// <summary>
        /// On update, move at the mouse (for cursors)
        /// </summary>
        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (null == mouseDevice || !mouseDevice.IsAvailable()) { return; }

            var target = gameObject;
            if (null == target) { return; }

            var worldPosition = mouseDevice.WorldPosition;
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
