using System;
using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/17/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Animates values in a loop/once/ping-ping animation cycle
    /// </summary>
    public class CycleAnimator<T> : Updatable
    {
        public enum ReverseType
        {
            /// <summary>
            /// On reverse direction, apply the interpolation of the animation curve in reverse
            /// </summary>
            Rewind,

            /// <summary>
            /// On reverse direction, match the interpolation of the animation curve used in the forward direction
            /// </summary>
            Match
        }

        /// <summary>
        /// Interpolation, start, and end values
        /// </summary>
        public Interpolator<T> interpolator;

        /// <summary>
        /// Cycle progress for Once, Loop, PingPong
        /// </summary>
        public AnimationCycleTimer timer;

        /// <summary>
        /// Value binding to modify value
        /// </summary>
        public SetBinding<T> binding;

        public ReverseType reverseType = ReverseType.Match;

        public CycleAnimator(Interpolator<T> interpolator, AnimationCycleTimer timer, SetBinding<T> binding)
        {
            this.interpolator = interpolator;
            this.timer = timer;
            this.binding = binding;
        }

        public CycleAnimator(Interpolator<T> interpolator, AnimationCycleTimer timer, Binding<T> binding)
        {
            this.interpolator = interpolator;
            this.timer = timer;
            this.binding = new SetBinding<T>(binding.setAction);
        }

        public float Progress
        {
            get => timer.Progress;
        }

        public bool IsFinished
        {
            get => timer.IsFinished;
        }

        public void OnUpdate(TimeSlice time)
        {
            if (IsFinished) { return; }

            timer.OnUpdate(time);

            var curveValue = interpolator.ValueAt(Progress);

            switch (timer.CycleState)
            {
                case AnimationCycleState.Forward:
                    break;
                case AnimationCycleState.Reverse:
                    switch (reverseType)
                    {
                        case ReverseType.Rewind:
                            break;
                        case ReverseType.Match:
                            var reverseCurve = new Interpolator<T>(interpolator.end, interpolator.start, interpolator.valueInterpolator, interpolator.transform);
                            curveValue = reverseCurve.ValueAt(1.0f - Progress);
                            break;
                    }
                    break;
            }

            binding.Value = curveValue;
        }
    }
}
