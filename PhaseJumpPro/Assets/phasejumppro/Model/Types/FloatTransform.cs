using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 4/13/122
 */
namespace PJ
{
    public abstract class FloatTransform : SomeTransform<float>
    {
    }

    public class FloatMultiplyTransform : FloatTransform
    {
        public float factor;

        public override float Transform(float value)
        {
            return value * factor;
        }
    }
}
