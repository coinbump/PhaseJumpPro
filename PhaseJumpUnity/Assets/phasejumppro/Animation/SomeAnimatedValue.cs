using System;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/31/22
*/
namespace PJ
{
    /// <summary>
    /// An animated value changes from 0-1.0 normalized progress
    /// </summary>
    public abstract class SomeAnimatedValue<T> : SomeTransform<float, T>
    {
        public override T Transform(float key) { return ValueAt(key); }

        public abstract T ValueAt(float progress);
    }
}
