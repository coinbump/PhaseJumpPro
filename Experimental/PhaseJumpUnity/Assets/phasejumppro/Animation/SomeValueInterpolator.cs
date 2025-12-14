using System;
using UnityEngine;

/*
RATING: 5 stars
Simple utility code
CODE REVIEW: 11/26/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Interpolates a value from 0-1 progress
    /// </summary>
    public abstract class SomeValueInterpolator<T>
    {
        public abstract T ValueAt(T start, T end, float progress);
    }

    /// <summary>
    /// Interpolates a float value
    /// </summary>
    public class FloatValueInterpolator : SomeValueInterpolator<float>
    {
        public override float ValueAt(float start, float end, float progress)
        {
            return start + (end - start) * progress;
        }
    }

    /// <summary>
    /// Interpolates a Vector2 value
    /// </summary>
    public class Vector2ValueInterpolator : SomeValueInterpolator<Vector2>
    {
        public override Vector2 ValueAt(Vector2 start, Vector2 end, float progress)
        {
            return start + (end - start) * progress;
        }
    }

    /// <summary>
    /// Interpolates a Vector3 value
    /// </summary>
    public class Vector3ValueInterpolator : SomeValueInterpolator<Vector3>
    {
        public override Vector3 ValueAt(Vector3 start, Vector3 end, float progress)
        {
            return start + (end - start) * progress;
        }
    }
}
