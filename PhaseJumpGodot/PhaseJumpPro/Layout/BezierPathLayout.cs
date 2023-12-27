using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/23/22
 */
namespace PJ
{
    /// <summary>
    /// Distribute objects along a circle path
    /// </summary>
    public partial class BezierPathLayout : SomePathLayout
    {
        public List<Vector3> controlPoints = new();

        protected override SomePath BuildPath()
        {
            return new BezierPath(controlPoints);
        }
    }
}
