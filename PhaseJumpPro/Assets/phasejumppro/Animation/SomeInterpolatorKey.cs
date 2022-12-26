using UnityEngine;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 12/17/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Produces an interpolator based on key values
    /// </summary>
    public abstract class SomeInterpolatorKey<T> {
        public abstract Interpolator<T> NewInterpolator(T start, T end, SomeInterpolatorKey<T> nextKey);
    }
    
    /// <summary>
    /// Allows us to set an interpolation curve from one keyframe to the next
    /// FUTURE: support Unity-style tangent/weight type animation curves
    /// </summary>
    public class InterpolateInterpolatorKey : SomeInterpolatorKey<float> {
        SomeInterpolate interpolate;

        public override Interpolator<float> NewInterpolator(float start, float end, SomeInterpolatorKey<float> nextKey)
        {
            return new(start, end, new FloatValueInterpolator(), interpolate);
        }
    }

    // FUTURE: support SplineAnimationCurve
}
