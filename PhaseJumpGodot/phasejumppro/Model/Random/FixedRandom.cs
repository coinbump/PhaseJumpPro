using System;
using System.Collections.Generic;
using Godot;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 1/19/23
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Returns a fixed set of random values (for testing, debugging)
    /// </summary>
    public class FixedRandom : SomeRandom
    {
        public List<float> values = new List<float>();

        protected int valueIndex = 0;

        public override float Value
        {
            get
            {
                if (values.Count == 0) { return 0; }

                var index = valueIndex % values.Count;
                valueIndex++;
                return values[index];
            }
        }

        public override int RangeValue(int min, int max)
        {
            return Mathf.FloorToInt((float)min + Mathf.Min(.99f, Value) * (float)(max - min));
        }

        public override void SetSeed(int seed)
        {
        }
    }
}
