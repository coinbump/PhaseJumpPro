using System;
using System.Collections.Generic;
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
    public class BezierPathLayout : SomePathLayout
    {
        public List<Vector3> controlPoints;

        protected override SomePath NewPath()
        {
            return new BezierPath(controlPoints);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawPath(NewPath(), EditorUtils.DefaultPathStep, transform, renderState);
        }

        [CustomEditor(typeof(BezierPathLayout))]
        public new class Editor : SomeGameObjectsLayout.Editor
        {
        }
#endif
    }
}
