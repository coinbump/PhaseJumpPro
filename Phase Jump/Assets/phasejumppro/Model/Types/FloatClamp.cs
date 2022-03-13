using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Clamp a float
    /// </summary>
    public class FloatClamp : SomeTransform<float>
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
