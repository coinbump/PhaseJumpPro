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
            var oldTime = this.time;

            base.OnUpdate(time);

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
                // If we only have 1 keyframe, use that keyframe value until we can interpolate between start, end
                if (null != keyframeBefore)
                {
                    binding.Value = keyframeBefore.value;
                }
                else if (null != keyframeAfter)
                {
                    binding.Value = keyframeAfter.value;
                }
                return;
            }

            var totalDelta = keyframeAfter.time - keyframeBefore.time;
            var delta = newTime - keyframeBefore.time;
            var progress = MathF.Min(1.0f, delta / totalDelta);
            T startValue = keyframeBefore.value;
            T endValue = keyframeAfter.value;

            // Each keyframe may optionally define its own interpolation curve to the next keyframe
            // This interpolator is created via an Interpolator Key object
            if (null != keyframeBefore.interpolatorFactory)
            {
                var interpolator = keyframeBefore.interpolatorFactory.NewInterpolator(startValue, endValue, keyframeAfter.interpolatorFactory);
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
