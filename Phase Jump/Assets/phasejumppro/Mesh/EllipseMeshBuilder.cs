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
    /// Build a Ellipse mesh
    /// </summary>
    public class EllipseMeshBuilder : SomeMeshBuilder
    {
        public float angleStep = 10.0f;
        public Vector2 size = new Vector2(1.0f, 1.0f);

        public override Mesh BuildMesh()
        {
            var shapeMesh = new EllipseMesh(angleStep, size);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(EllipseMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    EllipseMeshBuilder EllipseMeshBuilder = (EllipseMeshBuilder)target;
                    EllipseMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
