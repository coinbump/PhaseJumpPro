﻿using UnityEditor;
using UnityEngine;
using System.Collections;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
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

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var planeMesh = new SeparatedPlaneMesh(meshSize, worldSize);
            var mesh = planeMesh.Build(true);

            // Randomly offset the mesh triangles to demonstrate their separatedness.
            var newVertices = mesh.vertices;

            for (int i = 0; i < mesh.vertices.Length; i += 3)
            {
                var vertexOffset = new Vector3(new UnityRandom().VaryFloat(0, randomOffset.x), new UnityRandom().VaryFloat(0, randomOffset.y), new UnityRandom().VaryFloat(0, randomOffset.z));
                // Debug.Log("Vertex Offset: " + vertexOffset.ToString());

                // Offset each triangle individually
                newVertices[0 + i] += vertexOffset;
                newVertices[1 + i] += vertexOffset;
                newVertices[2 + i] += vertexOffset;
            }

            planeMesh.UpdateMesh(mesh, newVertices, mesh.triangles, mesh.uv);

            return mesh;
        }
    }
}
