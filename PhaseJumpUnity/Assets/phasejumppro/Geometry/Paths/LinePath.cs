using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// Model that defines a line path
    /// </summary>
    public class LinePath : SomePath
    {
        public Vector3 start;
        public Vector3 end;

        public LinePath(Vector3 start, Vector3 end)
        {
            this.start = start;
            this.end = end;
        }

        public override Vector3 PositionAt(float progress)
        {
            return start + (end - start) * progress;
        }
    }
}
