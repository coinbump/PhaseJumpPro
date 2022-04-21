using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Simple mesh
 * CODE REVIEW: 4/7/22
 */
namespace PJ
{
    /// <summary>
    /// Build a Convex poly mesh
    /// </summary>
    // FUTURE: add support for world size to scale polygon
    public class ConvexPolyMeshBuilder : SomeMeshBuilder
    {
        public Polygon polygon = new Polygon();

        public override Vector2 WorldSize
        {
            get => polygon.Size;
            set
            {
                Debug.Log("World size for convex poly not yet supported");
            }
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new ConvexPolyMesh(polygon);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(ConvexPolyMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    ConvexPolyMeshBuilder ConvexPolyMeshBuilder = (ConvexPolyMeshBuilder)target;
                    ConvexPolyMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
