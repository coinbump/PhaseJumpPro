using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 1/2/23
*/
namespace PJ
{
    /// <summary>
    /// Apply a text effect to a specific range of characters
    /// </summary>
    public abstract class SomeRangesTextEffect : SomeTextEffect
    {
        /// <summary>
        /// (EDITOR ONLY) serialization struct to define ranges
        /// </summary>
        [Serializable]
        public class Range
        {
            public int start = 0;
            public int count = 0;
        }

        /// <summary>
        /// (EDITOR ONLY) list of ranges
        /// </summary>
        [SerializeField]
        private List<Range> _ranges = new();

        protected List<PJ.Range<int>> ranges = new();

        protected override void Awake()
        {
            base.Awake();

            // Convert editor ranges
            foreach (var range in _ranges)
            {
                var endValue = range.start + range.count - 1;
                if (endValue >= range.start)
                {
                    ranges.Add(PJ.Range<int>.InclusiveRange(range.start, endValue));
                }
            }
            _ranges.Clear();
        }
    }
}
