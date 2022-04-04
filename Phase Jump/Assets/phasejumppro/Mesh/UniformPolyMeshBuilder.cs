﻿using UnityEditor;
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
    public class UniformPolyMeshBuilder : SomeMeshBuilder
    {
        public int vertexCount = 3;
        public float radius = 1.0f;

        public override Mesh BuildMesh()
        {
            var shapeMesh = new UniformPolyMesh(vertexCount, radius);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(UniformPolyMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    UniformPolyMeshBuilder UniformPolyMeshBuilder = (UniformPolyMeshBuilder)target;
                    UniformPolyMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
