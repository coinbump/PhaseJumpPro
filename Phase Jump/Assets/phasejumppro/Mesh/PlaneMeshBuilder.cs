using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    /// <summary>
    /// Build a plane mesh of NxN size and replace the current mesh
    /// Requires that a MeshFilter component be added to the gameObject (usually a plane)
    /// </summary>
    /// Reference: https://docs.unity3d.com/Manual/Example-CreatingaBillboardPlane.html
    public class PlaneMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;

        public override Mesh BuildMesh()
        {
            var planeMesh = new PlaneMesh(meshSize, worldSize, faceAxis);
            var mesh = planeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(PlaneMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    PlaneMeshBuilder planeMeshBuilder = (PlaneMeshBuilder)target;
                    planeMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
