using UnityEngine;

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
