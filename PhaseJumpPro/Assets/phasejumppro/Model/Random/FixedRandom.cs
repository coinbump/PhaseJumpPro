using System;
using System.Collections.Generic;
using UnityEngine;

namespace PJ
{
    /// <summary>
    /// Returns a fixed set of random values (for testing, debugging)
    /// </summary>
    public class FixedRandom : SomeRandom
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
