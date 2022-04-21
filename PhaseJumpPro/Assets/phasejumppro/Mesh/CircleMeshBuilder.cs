using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 */
namespace PJ
{
    /// <summary>
    /// Build a circle mesh
    /// </summary>
    public class CircleMeshBuilder : SomeMeshBuilder
    {
        public float angleStep = 10.0f;
        public float radius = 1.0f;

        public override Vector2 WorldSize
        {
            get => new Vector2(radius * 2.0f, radius * 2.0f);
            set
            {
                var size = Mathf.Min(value.x, value.y);
                radius = size / 2.0f;
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new CircleMesh(angleStep, radius);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(CircleMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    CircleMeshBuilder CircleMeshBuilder = (CircleMeshBuilder)target;
                    CircleMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
