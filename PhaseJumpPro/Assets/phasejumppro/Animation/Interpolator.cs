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
    /// Interpolates a value from start to end, with an interpolation transform
    /// </summary>
    public class Interpolator<T>
    {
        public T start;
        public T end;
        public SomeValueInterpolator<T> valueInterpolator;
        public SomeValueTransform<float> transform;

        public Interpolator(T start, T end, SomeValueInterpolator<T> valueInterpolator, SomeValueTransform<float> transform = null) {
            this.start = start;
            this.end = end;
            this.valueInterpolator = valueInterpolator;

            this.transform = (null != transform) ? transform : _InterpolateType.linear;
        }
        
        public T ValueAt(float progress) {
            return valueInterpolator.ValueAt(start, end, transform.Transform(progress));
        }
    }
}
