using UnityEditor;
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
    /// Build a quad mesh of NxN size and replace the current mesh
    /// </summary>
    public class QuadMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int meshSize = new Vector2Int(1, 1);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var quadMesh = new QuadMesh(meshSize, worldSize);
            var mesh = quadMesh.Build(true);
            return mesh;
        }
    }
}
