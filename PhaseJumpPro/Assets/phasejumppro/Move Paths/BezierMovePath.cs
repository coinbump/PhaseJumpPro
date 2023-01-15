using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/31/22
 */
namespace PJ
{
    /// <summary>
    /// Moves objects around a circle radius
    /// FUTURE: support rotated ellipses, sin paths, etc.
    /// </summary>
    public class BezierMovePath : SomeMovePath
    {
        public List<Vector3> controlPoints = new List<Vector3>();

        protected override SomePath BuildPath()
        {
            return new BezierPath(controlPoints);
        }
    }
}
