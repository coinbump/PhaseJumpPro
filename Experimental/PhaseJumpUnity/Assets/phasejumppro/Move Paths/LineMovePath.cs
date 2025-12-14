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
    /// Moves objects on a line in 3D (or 2D) space
    /// </summary>
    public class LineMovePath : SomeMovePath
    {
        public Vector3 start;
        public Vector3 end;

        protected override SomePath BuildPath()
        {
            return new LinePath(start, end);
        }

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            var worldStart = transform.TransformPoint(start);
            var worldEnd = transform.TransformPoint(end);

            EditorUtils.DrawLine(worldStart, worldEnd, renderState);
        }
#endif
    }
}
