using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
    /// <summary>
    /// Moves objects around a circle radius
    /// FUTURE: support rotated ellipses, sin paths, etc.
    /// </summary>
    public class CircleMovePath2D : MovePath
    {
        public float radius = 1.0f;

        protected override void Awake()
        {
            base.Awake();

            path = new CirclePath(radius);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }
#endif
    }
}
