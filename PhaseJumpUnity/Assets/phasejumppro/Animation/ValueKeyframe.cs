using UnityEngine;

/*
RATING: 5 stars
Simple types
CODE REVIEW: 12/17/22
*/
namespace PJ
{
    public class ValueKeyframe<T> : SomeKeyframe
    {
        public T value;

        /// <summary>
        /// Produces the interpolator for the animation curve
        /// </summary>
        public SomeKeyframeInterpolatorFactory<T> interpolatorFactory;

        public ValueKeyframe(float time, T value) : base(time)
        {
            this.value = value;
        }
    }
}
