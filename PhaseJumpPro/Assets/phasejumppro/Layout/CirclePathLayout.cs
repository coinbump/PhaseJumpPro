using System;
using UnityEditor;
using UnityEngine;

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
    public class CirclePathLayout : SomePathLayout
    {
        public float radius = 1.0f;

        protected override SomePath NewPath()
        {
            return new CirclePath(radius);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }

        [CustomEditor(typeof(CirclePathLayout))]
        public new class Editor : SomeGameObjectsLayout.Editor
        {
        }
#endif
    }
}
