﻿using UnityEditor;
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
    /// Build a PolyFrame mesh
    /// </summary>
    public class PolyFrameMeshBuilder : SomeMeshBuilder
    {
        public bool isClosed = false;
        public float strokeWidth = 0.1f;
        public Polygon polygon = new Polygon();

        public override Mesh BuildMesh()
        {
            var shapeMesh = new PolyFrameMesh(polygon, isClosed, strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }

#if UNITY_EDITOR
        [CustomEditor(typeof(PolyFrameMeshBuilder))]
        public class Editor : UnityEditor.Editor
        {
            public override void OnInspectorGUI()
            {
                DrawDefaultInspector();

                if (GUILayout.Button("Build"))
                {
                    PolyFrameMeshBuilder PolyFrameMeshBuilder = (PolyFrameMeshBuilder)target;
                    PolyFrameMeshBuilder.Build();
                }
            }
        }
#endif
    }
}
