using System.Collections.Generic;
using Godot;
using System;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Vary a vector angle in 2D space
    /// </summary>
    public class VaryVectorAngle2D : SomeValueTransform<Vector3>
    {
        public Angle varyAngle;

        public VaryVectorAngle2D(Angle varyAngle)
        {
            this.varyAngle = varyAngle;
        }

        public override Vector3 Transform(Vector3 value)
        {
            var angle = new Angle(new Vector2(value.X, value.Y));
            var newAngle = Angle.DegreesAngle(new EngineRandom().VaryFloat(angle.Degrees, varyAngle.Degrees));
            var newAngleVector = (Vector2)newAngle;
            var result = newAngleVector.Normalized() * value.Length();
            return new(result.X, result.Y, 0);
        }
    }
}
