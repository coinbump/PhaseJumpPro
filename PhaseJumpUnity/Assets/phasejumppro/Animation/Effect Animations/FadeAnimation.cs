using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// Fades in over N duration time.
    /// </summary>
    public class FadeAnimation : SomeAnimation
    {
        public float duration = 1.0f;

        [Range(0, 1.0f)]
        public float startAlpha = 0.0f;

        [Range(0, 1.0f)]
        public float endAlpha = 1.0f;

        /// <summary>
        /// If true, turn off collisions while the animation is running
        /// </summary>
        public bool disableCollisions = true;

        protected Timer timer = new Timer(1.0f, SomeRunner.RunType.RunOnce);
        protected bool wasColliderEnabled = true;
        protected Interpolator<float> interpolator;
        protected MultiCollider multiCollider;

        protected override void Awake()
        {
            base.Awake();

            interpolator = new(startAlpha, endAlpha, new FloatValueInterpolator());

            timer.duration = duration;
            multiCollider = new MultiCollider(gameObject);

            if (!enabled) { return; }

            MultiRenderer.Alpha = interpolator.ValueAt(0);

            wasColliderEnabled = multiCollider.Enabled;
            if (disableCollisions)
            {
                multiCollider.Enabled = false;
            }
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (timer.IsFinished) { return; }
            timer.OnUpdate(time);

            if (timer.IsFinished)
            {
                // Stop receiving update events, animation is finished
                enabled = false;

                if (disableCollisions)
                {
                    multiCollider.Enabled = wasColliderEnabled;
                }
            }

            MultiRenderer.Alpha = interpolator.ValueAt(timer.Progress);
        }
    }
}
