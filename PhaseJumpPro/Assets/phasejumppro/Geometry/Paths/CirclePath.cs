using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple path
 * CODE REVIEW: 4/23/22
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

        public Vector3 PositionAt(float position)
        {
            var result = AngleUtils.DegreeAngleToVector2(position * 360.0f, radius);
            return result;
        }
    }
}
