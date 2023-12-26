using System;
using Godot;
using System.Collections.Generic;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 2/18/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Animates a value from start to end once once
    /// </summary>
    public class Animator<T> : SomeTimed
    {
        /// <summary>
        /// Time value of animator
        /// </summary>
        protected float time;

        /// <summary>
        /// Interpolates from start to end
        /// </summary>
        public Interpolator<T> interpolator;

        /// <summary>
        /// Value binding to modify value
        /// </summary>
        public SetBinding<T> binding;

        public Animator(Interpolator<T> interpolator, float duration, SetBinding<T> binding) : base(duration, SomeRunner.RunType.RunOnce)
        {
            this.interpolator = interpolator;
            this.binding = binding;
        }

        public override float Progress => Mathf.Max(0, Mathf.Min(1.0f, time / duration));

        public override void OnUpdate(TimeSlice time)
        {
            if (IsFinished) { return; }

            this.time += time.delta;
            IsFinished = this.time >= duration;

            var curveValue = interpolator.ValueAt(Progress);
            binding.Value = curveValue;
        }
    }
}
