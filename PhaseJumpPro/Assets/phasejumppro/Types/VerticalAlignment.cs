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
    /// Vertical alignment in a horizontal flow
    /// </summary>
    public struct VerticalAlignment
    {
        public SomeAligner aligner;
        
        public VerticalAlignment(SomeAligner aligner)
        {
            this.aligner = aligner;
        }

        public static VerticalAlignment top = new VerticalAlignment(new TopAligner());
        public static VerticalAlignment center = new VerticalAlignment(new CenterAligner());
        public static VerticalAlignment bottom = new VerticalAlignment(new BottomAligner());
    }

    public struct TopAligner : SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize)
        {
            return 0;
        }
    }

    public struct BottomAligner : SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize)
        {
            return layoutSize - frameSize;
        }
    }
}
