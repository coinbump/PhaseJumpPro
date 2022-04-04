using UnityEditor;
using UnityEngine;
using System.Collections;

namespace PJ
{
    /// <summary>
    /// Build a plane mesh of NxN size and replace the current mesh
    /// </summary>
    public class SeparatedPlaneMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Vector3 randomOffset = new Vector3(0, 0, 0);

        public override Mesh BuildMesh()
        {
            var planeMesh = new SeparatedPlaneMesh(meshSize, worldSize);
            var mesh = planeMesh.Build(true);

            // Randomly offset the mesh triangles to demonstrate their separatedness.
            var newVertices = mesh.vertices;

            for (int i = 0; i < mesh.vertices.Length; i += 3)
            {
                var vertexOffset = new Vector3(PJ.RandomUtils.VaryFloat(0, randomOffset.x), PJ.RandomUtils.VaryFloat(0, randomOffset.y), PJ.RandomUtils.VaryFloat(0, randomOffset.z));
                Debug.Log("Vertex Offset: " + vertexOffset.ToString());

                // Offset each triangle individually
                newVertices[0 + i] += vertexOffset;
                newVertices[1 + i] += vertexOffset;
                newVertices[2 + i] += vertexOffset;
            }

            planeMesh.UpdateMesh(mesh, newVertices, mesh.triangles, mesh.uv);

            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(SeparatedPlaneMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    SeparatedPlaneMeshBuilder meshBuilder = (SeparatedPlaneMeshBuilder)target;
                    meshBuilder.Build();
                }
            }
        }
#endif
    }
}
