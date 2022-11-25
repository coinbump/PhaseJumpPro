using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/7/22
 */
namespace PJ
{
    /// <summary>
    /// Build a Grid mesh
    /// </summary>
    public class GridMeshBuilder : SomeMeshBuilder
    {
        public Vector2Int gridSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(3.0f, 3.0f);
        public Vector2 cellSize = new Vector2(0, 0);
        public float strokeWidth = 0.1f;

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new GridMesh(gridSize, worldSize, cellSize, strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
