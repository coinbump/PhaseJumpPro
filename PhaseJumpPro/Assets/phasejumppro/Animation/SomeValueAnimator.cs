using System;
using UnityEngine;

/*
RATING: 5 stars
Simple utilty code
CODE REVIEW: 11/26/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Animates a value from 0-1 progress
    /// </summary>
    public abstract class SomeValueAnimator<T>
    {
        public abstract T ValueAt(T start, T end, float progress);
    }

    /// <summary>
    /// Animates a float value
    /// </summary>
    public class FloatValueAnimator : SomeValueAnimator<float>
    {
        public override float ValueAt(float start, float end, float progress)
        {
            return start + (end - start) * progress;
        }
    }

    /// <summary>
    /// Animates a Vector2 value
    /// </summary>
    public class Vector2ValueAnimator : SomeValueAnimator<Vector2>
    {
        public override Vector2 ValueAt(Vector2 start, Vector2 end, float progress)
        {
            return start + (end - start) * progress;
        }
    }
}
