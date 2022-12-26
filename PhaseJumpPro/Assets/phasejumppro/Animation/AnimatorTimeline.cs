using System;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/18/22
*/
namespace PJ
{
    /// <summary>
    /// Animates values across timeline keyframes
    /// </summary>
    public class AnimatorTimeline<T> : Timeline
    {
        public SomeValueInterpolator<T> valueInterpolator;
        public Binding<T> binding;

        /// <summary>
        /// OPTIONAL: produces values based on an expression
        /// </summary>
        public Func<float, T> expression;

        public AnimatorTimeline(SomeValueInterpolator<T> valueInterpolator, Binding<T> binding, Func<float, T> expression = null)
        {
            this.valueInterpolator = valueInterpolator;
            this.binding = binding;
            this.expression = expression;
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            var oldTime = this.time;
            this.time += time.delta;
            var newTime = this.time;

            // If an expression exists, use that to determine values
            if (null != expression)
            {
                float expressionProgress = MathF.Min(1.0f, (newTime - startTime) / duration);
                binding.Value = expression(expressionProgress);
                return;
            }

            var keyframeBefore = KeyframeBefore(newTime) as ValueKeyframe<T>;
            var keyframeAfter = KeyframeAfterOrAt(newTime) as ValueKeyframe<T>;
            if (null == keyframeBefore || null == keyframeAfter)
            {
                if (null != keyframeBefore)
                {
                    binding.Value = keyframeBefore.value;
                }
                return;
            }

            var totalDelta = keyframeAfter.time - keyframeBefore.time;
            var delta = newTime - keyframeBefore.time;
            var progress = MathF.Min(1.0f, delta / totalDelta);
            T startValue = keyframeBefore.value;
            T endValue = keyframeAfter.value;

            if (null != keyframeBefore.interpolatorKey)
            {
                var interpolator = keyframeBefore.interpolatorKey.NewInterpolator(startValue, endValue, keyframeAfter.interpolatorKey);
                var value = interpolator.ValueAt(progress);
                binding.Value = value;
            }
            else
            {
                var interpolator = new Interpolator<T>(startValue, endValue, valueInterpolator, null);
                var value = interpolator.ValueAt(progress);
                binding.Value = value;
            }
        }
    }
}
