using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Simple utility
CODE REVIEW: 12/29/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Returns a fixed set of random values (for testing, debugging)
    /// </summary>
    public class FixedNormalRandom : SomeNormalRandom
    {
        public List<float> values = new List<float>();

        protected int valueIndex = 0;

        public float Value
        {
            get
            {
                if (values.Count == 0) { return 0; }

                var index = valueIndex % values.Count;
                valueIndex++;
                return values[index];
            }
        }
    }
}
