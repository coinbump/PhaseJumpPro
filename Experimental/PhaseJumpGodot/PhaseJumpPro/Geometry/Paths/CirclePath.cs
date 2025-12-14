using System;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
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
            return new(result.X, result.Y, 0);
        }
    }
}
