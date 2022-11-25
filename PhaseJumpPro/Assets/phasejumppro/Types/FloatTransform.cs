using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 4/13/22
 * 
 * PORTED TO: C++
 */
namespace PJ
{
    public abstract class SomeFloatTransform : SomeTransform<float>
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
