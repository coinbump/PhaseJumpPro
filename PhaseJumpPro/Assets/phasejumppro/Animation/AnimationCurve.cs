using System;
using UnityEngine;

/*
RATING: 5 stars
Utility code
CODE REVIEW: 11/26/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Specifies the details of an animation curve from start to end, with an interpolation transform
    /// </summary>
    public class AnimationCurve<T>
    {
        public T start;
        public T end;
        public SomeValueAnimator<T> valueAnimator;
        public SomeTransform<float> transform;

        public AnimationCurve(T start, T end, SomeValueAnimator<T> valueAnimator, SomeTransform<float> transform = null) {
            this.start = start;
            this.end = end;
            this.valueAnimator = valueAnimator;

            this.transform = (null != transform) ? transform : Interpolate.linear;
        }
        
        public T ValueAt(float progress) {
            return valueAnimator.ValueAt(start, end, transform.Transform(progress));
        }
    }
}
