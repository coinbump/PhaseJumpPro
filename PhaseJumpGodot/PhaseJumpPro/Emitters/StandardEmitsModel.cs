using System.Collections.Generic;
using Godot;
using System;

/*
RATING: 5 stars
Has Unit tests
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// Standard model for describing how to create a list of emits for an emitter
    /// </summary>
    public class StandardEmitsModel
    {
        public class EmitModel
        {
            public Vector3 velocity = Vector3.Zero;
            public Vector3 offset = Vector3.Zero;
            public SomeValueTransform<Vector3> velocityTransform;
            public float delay = 0;
            public float lifeTime = 0;
        }

        public List<EmitModel> models = new();

        public void Clear()
        {
            models.Clear();
        }

        /// <summary>
        /// Emit N objects, spread by N degrees each, centered to 0
        /// </summary>
        public void BuildSpread2D(int emitCount, Angle angleStep, Angle varyAngle)
        {
            Clear();

            if (0 == emitCount) { return; }

            var firstAngle = -angleStep.Degrees * ((float)(emitCount - 1) / 2);

            for (int i = 0; i < emitCount; i++)
            {
                var model = new EmitModel();
                var velocity = Angle.DegreesAngle(firstAngle + i * angleStep.Degrees).ToVector2();
                model.velocity = new(velocity.X, velocity.Y, 0);
                model.velocityTransform = new VaryVectorAngle2D(varyAngle);
                models.Add(model);
            }
        }

        public float VelocityMagnitude
        {
            set
            {
                for (int i = 0; i < models.Count; i++)
                {
                    var velocity = models[i].velocity.Normalized();
                    models[i].velocity = velocity * value;
                }
            }
        }
    }
}
