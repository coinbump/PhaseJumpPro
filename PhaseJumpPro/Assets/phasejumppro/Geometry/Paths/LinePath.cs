using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Model that defines a circle path
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

        public Vector3 PositionAt(float position)
        {
            return start + (end - start) * position;
        }
    }
}
