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
    /// Moves objects on a line in 3D (or 2D) space
    /// </summary>
    public class LineMovePath : SomeMovePath
    {
        public Vector3 start;
        public Vector3 end;

#if UNITY_EDITOR
        protected override void RenderGizmos(EditorUtils.RenderState renderState)
        {
            var worldStart = transform.TransformPoint(start);
            var worldEnd = transform.TransformPoint(end);

            EditorUtils.DrawLine(worldStart, worldEnd, renderState);
        }
#endif

        protected override void SnapPathMover(PathMover pathMover, float progress, bool force)
        {
            Vector3 position = start + (end - start) * progress;
            MovePathMoverToLocalPosition(pathMover, position, force);
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(LineMovePath))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Snap"))
                {
                    LineMovePath movePath = (LineMovePath)target;
                    movePath.SnapAllToStartPosition();
                }
            }
        }
#endif
    }
}
