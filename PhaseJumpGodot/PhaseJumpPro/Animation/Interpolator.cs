using System;
using Godot;

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

        public Interpolator(T start, T end, SomeValueInterpolator<T> valueInterpolator, SomeValueTransform<float> transform = null)
        {
            this.start = start;
            this.end = end;
            this.valueInterpolator = valueInterpolator;

            this.transform = (null != transform) ? transform : _InterpolateType.linear;
        }

        public T ValueAt(float progress)
        {
            return valueInterpolator.ValueAt(start, end, transform.Transform(progress));
        }
    }

    public class FloatInterpolator : Interpolator<float>
    {
        public FloatInterpolator(float start, float end, SomeValueTransform<float> transform = null) : base(start, end, new FloatValueInterpolator(), transform)
        {
        }
    }

    public class Vector2Interpolator : Interpolator<Vector2>
    {
        public Vector2Interpolator(Vector2 start, Vector2 end, SomeValueTransform<float> transform = null) : base(start, end, new Vector2ValueInterpolator(), transform)
        {
        }
    }

    public class Vector3Interpolator : Interpolator<Vector3>
    {
        public Vector3Interpolator(Vector3 start, Vector3 end, SomeValueTransform<float> transform = null) : base(start, end, new Vector3ValueInterpolator(), transform)
        {
        }
    }
}
