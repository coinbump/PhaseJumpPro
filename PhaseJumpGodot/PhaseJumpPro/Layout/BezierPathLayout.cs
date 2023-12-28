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
        // TODO: Not supported by export, so how can we define the points?
        public List<Vector3> controlPoints = new();

        protected override SomePath BuildPath()
        {
            return new BezierPath(controlPoints);
        }
    }
}
