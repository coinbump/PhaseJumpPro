using System.Collections.Generic;
using UnityEngine;
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
            var angle = new Angle(new Vector2(value.x, value.y));
            var newAngle = Angle.DegreesAngle(RandomUtils.VaryFloat(angle.Degrees, varyAngle.Degrees));
            var newAngleVector = (Vector2)newAngle;
            return newAngleVector.normalized * value.magnitude;
        }
    }
}
