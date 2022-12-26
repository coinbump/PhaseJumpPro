using System;
using UnityEngine;

namespace PJ
{
    public enum FocusDirection
    {
        Up, Down, Left, Right
    }

    /// <summary>
    /// Determines the logic for changing focus from the current focusable to the next
    /// </summary>
    public abstract class SomeFocusCoordinator : WorldComponent
    {
        public virtual void MoveFocus(Vector2 direction)
        {
            var angleAxisLimiter = new AngleAxisLimiter2D(4);
            var limitedAngle = angleAxisLimiter.LimitAngle(new Angle(direction)).Degrees;

            if (limitedAngle == 0f)
            {
                MoveFocus(FocusDirection.Up);
            }
            else if (limitedAngle == 90.0f)
            {
                MoveFocus(FocusDirection.Right);
            }
            else if (limitedAngle == 180.0f)
            {
                MoveFocus(FocusDirection.Down);
            }
            else if (limitedAngle == 270.0f)
            {
                MoveFocus(FocusDirection.Left);
            }
        }

        public abstract void MoveFocus(FocusDirection direction);
    }
}
