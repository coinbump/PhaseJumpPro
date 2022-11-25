using UnityEditor;
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
    /// Build a Ellipse mesh
    /// </summary>
    public class EllipseMeshBuilder : SomeMeshBuilder
    {
        public float angleStep = 10.0f;
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new EllipseMesh(angleStep, worldSize);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
