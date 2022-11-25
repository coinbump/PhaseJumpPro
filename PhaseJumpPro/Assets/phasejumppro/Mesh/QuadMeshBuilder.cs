using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/12/22
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

        public override Vector2 WorldSize
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
