using UnityEngine;
using System;
using System.Collections.Generic;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Cannon emitter in 2D space
    /// Call fire to fire the cannon at a specific angle (limited by rate limiter in CannonModel)
    /// </summary>
    public class CannonEmitter2D : SomeCannonEmitter
    {
        protected Angle emitAngle = Angle.DegreesAngle(0);

        public virtual List<GameObject> Fire(Angle angle)
        {
            if (model.Fire())
            {
                this.emitAngle = angle;
                return Emit();
            }

            return new();
        }

        protected override Vector3 EmitVelocity(Emit emit)
        {
            // TBD: is there a better algorithm for this?
            var emitVector2 = (Vector2)emit.velocity;
            var emitAngle = new Angle(emitVector2);
            var thisEmitAngle = this.emitAngle;
            var resultAngle = Angle.DegreesAngle(thisEmitAngle.Degrees + emitAngle.Degrees);
            var result = (Vector2)resultAngle;
            result = result.normalized;
            result *= emitVector2.magnitude;
            return result;
        }
    }
}
