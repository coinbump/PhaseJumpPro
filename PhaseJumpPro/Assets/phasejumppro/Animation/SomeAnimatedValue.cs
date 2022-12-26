using System;
using UnityEngine;

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
