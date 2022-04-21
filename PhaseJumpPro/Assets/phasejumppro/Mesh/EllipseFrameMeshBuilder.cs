using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/4/22
 */
namespace PJ
{
    /// <summary>
    /// Build a Ellipse frame mesh
    /// </summary>
    public class EllipseFrameMeshBuilder : SomeMeshBuilder
    {
        public float angleStep = 10.0f;
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public float strokeWidth = 0.1f;

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new EllipseFrameMesh(angleStep, worldSize, strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(EllipseFrameMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    EllipseFrameMeshBuilder EllipseFrameMeshBuilder = (EllipseFrameMeshBuilder)target;
                    EllipseFrameMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
