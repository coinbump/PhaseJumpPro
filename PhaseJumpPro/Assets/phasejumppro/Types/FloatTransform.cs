using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 1/15/23
 * 
 * PORTED TO: C++
 */
namespace PJ
{
    public abstract class SomeFloatTransform : SomeValueTransform<float>
    {
    }

    public class FloatMultiplyTransform : SomeFloatTransform
    {
        public float factor;

        public override float Transform(float value)
        {
            return value * factor;
        }
    }
}
