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
    public class CircleMovePath2D : SomeMovePath
    {
        public float radius = 1.0f;

        protected override SomePath BuildPath()
        {
            return new CirclePath(radius);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }
#endif
    }
}
