using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 1/19/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Returns a normalized random value between 0-1.0
    /// This exists so we can inject specific random numbers for unit tests
    /// </summary>
    public abstract class SomeRandom
    {
        /// <summary>
        /// Value between 0-1.0
        /// </summary>
        public virtual float Value { get; }

        /// <summary>
        /// Return a random integer between min [inclusive] and max [exclusive]
        /// </summary>
        public abstract int RangeValue(int min, int max);

        /// <summary>
        /// Return a random integer between 0 and max - 1
        /// </summary>
        public virtual int IntValue(int max) { return RangeValue(0, max); }

        /// <summary>
        /// Set the random generation seed
        /// </summary>
        public abstract void SetSeed(int seed);

        public Type ChooseFrom<Type>(Type[] choices)
        {
            if (choices.Length == 0) { return default; }

            int index = RangeValue(0, choices.Length);
            return choices[index];
        }

        public Type ChooseFrom<Type>(List<Type> choices)
        {
            if (choices.Count == 0) { return default; }

            int index = RangeValue(0, choices.Count);
            return choices[index];
        }

        public float VaryFloat(float value, float vary)
        {
            if (0 == vary) { return value; }
            return value + Delta(vary);
        }

        /// <summary>
        /// Returns a random value between -delta...delta.
        /// </summary>
        public float Delta(float delta)
        {
            bool isVaryForward = Value < 0.5f;
            float factor = isVaryForward ? 1.0f : -1.0f;
            return Value * factor * delta;
        }
    }
}
