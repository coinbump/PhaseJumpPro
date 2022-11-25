using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/27/22
 */
namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    public class LinePathLayout : SomePathLayout
    {
        public Vector3 start;
        public Vector3 end;

        protected override SomePath NewPath()
        {
            return new LinePath(start, end);
        }
    }
}
