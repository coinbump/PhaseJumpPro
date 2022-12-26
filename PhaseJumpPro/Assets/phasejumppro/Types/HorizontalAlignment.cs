using System;
using UnityEngine;

/*
RATING: 5 stars
Simple type
CODE REVIEW: 11/24/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Horizontal alignment in a vertical flow
    /// </summary>
    public struct HorizontalAlignment
    {
        public SomeAligner aligner;

        public HorizontalAlignment(SomeAligner aligner)
        {
            this.aligner = aligner;
        }

        public static HorizontalAlignment leading = new HorizontalAlignment(new LeadingAligner());
        public static HorizontalAlignment center = new HorizontalAlignment(new CenterAligner());
        public static HorizontalAlignment trailing = new HorizontalAlignment(new TrailingAligner());
    }

    public struct LeadingAligner : SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize)
        {
            return 0;
        }
    }

    public struct TrailingAligner : SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize)
        {
            return layoutSize - frameSize;
        }
    }
}
