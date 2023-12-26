using System;
using UnityEngine;

/*
RATING: 5 stars
Simple interface
CODE REVIEW: 12/7/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Aligns a view inside another view
    /// This uses reading coordinates (positive Y is down)
    /// </summary>
    public interface SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize);
    }

    public struct CenterAligner : SomeAligner
    {
        public float AlignedOrigin(float layoutSize, float frameSize)
        {
            return layoutSize / 2.0f - frameSize / 2.0f;
        }
    }
}
