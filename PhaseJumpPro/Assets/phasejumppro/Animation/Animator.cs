using System;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 11/26/22
 */
namespace PJ
{
    /// <summary>
    /// Performs an interpolated property change over time
    /// </summary>
    public class Animator<T> : Updatable
    {
        /// <summary>
        /// Interpolation, start, and end values
        /// </summary>
        public AnimationCurve<T> curve;

        /// <summary>
        /// Cycle progress for Once, Loop, PingPong
        /// </summary>
        public AnimationCycleTimer timer;

        /// <summary>
        /// Value binding to modify value
        /// </summary>
        public Binding<T> binding;

        public Animator(AnimationCurve<T> curve, AnimationCycleTimer timer, Binding<T> binding)
        {
            this.curve = curve;
            this.timer = timer;
            this.binding = binding;
        }

        public float Progress {
            get => timer.Progress;
        }

        public bool IsFinished {
            get => timer.IsFinished;
        }

        public void OnUpdate(TimeSlice time)
        {
            if (IsFinished) { return; }

            timer.OnUpdate(time);

            var curveValue = curve.ValueAt(Progress);
            binding.Value = curveValue;
        }
    }
}
