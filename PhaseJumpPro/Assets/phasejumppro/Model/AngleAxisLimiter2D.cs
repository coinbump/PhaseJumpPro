using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
    /// <summary>
    /// Limits a degree angle to a direct axis angle
    /// (used for 4-way, N-way movement in 2D space)
    /// </summary>
    public class AngleAxisLimiter2D
    {
        public int axisLimit = 0;

        public AngleAxisLimiter2D(int axisLimit)
        {
            this.axisLimit = axisLimit;
        }

        public float LimitAngle(float angle)
        {
            if (axisLimit <= 0) { return angle; }

            if (angle < 0)
            {
                return 0;
            }

            // If there are 4 axes available, then angles -45 to 45 are up (90 degrees total)
            var sliceAngle = 360.0f / axisLimit;
            angle = Mathf.Round(angle / sliceAngle);
            return angle * sliceAngle;
        }
    }
}
