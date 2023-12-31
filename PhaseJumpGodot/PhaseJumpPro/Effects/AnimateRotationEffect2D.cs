using Godot;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple animation
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    public partial class AnimateRotationEffect2D : SomeEffect
    {
        /// <summary>
        /// Angles per second
        /// </summary>
        [Export]
        public float anglesPerSecond = 360.0f;

        protected Node2D target;

        public override void Start()
        {
            target = Target as Node2D;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }

            var rotationDegrees = target.RotationDegrees;
            rotationDegrees += anglesPerSecond * time.delta;
            target.RotationDegrees = rotationDegrees;
        }
    }
}
