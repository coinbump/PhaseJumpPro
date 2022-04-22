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
    public class CircleMovePath2D : SomeMovePath
    {
        public float radius = 1.0f;

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            EditorUtils.DrawCircle(transform.position, radius, renderState);
        }
#endif

        protected override void SnapPathMover(PathMover pathMover, float progress, bool force)
        {
            var vector = AngleUtils.DegreeAngleToVector2(progress * 360.0f, radius);
            MovePathMoverToLocalPosition(pathMover, vector, force);
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(CircleMovePath2D))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Snap"))
                {
                    CircleMovePath2D movePath = (CircleMovePath2D)target;
                    movePath.SnapAllToStartPosition();
                }
            }
        }
#endif
    }
}
