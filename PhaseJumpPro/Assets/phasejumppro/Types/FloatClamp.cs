using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 11/5/22
 * 
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Clamp a float
    /// </summary>
    public class FloatClamp : SomeValueTransform<float>
    {
        public float minValue;
        public float maxValue;

        public FloatClamp(float minValue, float maxValue)
        {
            this.minValue = minValue;
            this.maxValue = maxValue;
        }

        public override float Transform(float value)
        {
            return Mathf.Clamp(value, minValue, maxValue);
        }
    }
}
