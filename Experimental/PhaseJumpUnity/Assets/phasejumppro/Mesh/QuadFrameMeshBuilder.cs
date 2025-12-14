using UnityEditor;
using UnityEngine;
using System.Collections;

/*
RATING: 5 stars
Hase unit tests
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Build a plane mesh of NxN size and replace the current mesh
    /// </summary>
    public class QuadFrameMeshBuilder : SomeMeshBuilder
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Vector2 strokeSize = new Vector2(0.1f, 0.1f);

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var polyMesh = new QuadFrameMesh(worldSize, strokeSize);
            var mesh = polyMesh.Build(true);
            return mesh;
        }
    }
}
