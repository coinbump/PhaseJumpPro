using System;

namespace PJ
{
    /// <summary>
    /// Specifies the details of an animation curve from start to end, with an interpolation transformer
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class AnimationCurve<T>
    {
        public T start;
        public T end;
        public float duration;
        public SomeTransform<float> curve = new InterpolateLinear();
    }
}
