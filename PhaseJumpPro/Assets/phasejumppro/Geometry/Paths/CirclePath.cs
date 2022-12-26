using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple path
 * CODE REVIEW: 4/23/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Model that defines a circle path
    /// </summary>
    public class CirclePath : SomePath
    {
        public float radius;

        public CirclePath(float radius)
        {
            this.radius = radius;
        }

        public override Vector3 PositionAt(float progress)
        {
            var result = Angle.DegreesAngle(progress * 360.0f).ToVector2(radius);
            return result;
        }
    }
}
