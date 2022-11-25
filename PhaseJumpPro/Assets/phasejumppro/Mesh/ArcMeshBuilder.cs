using UnityEditor;
using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/21/22
 */
namespace PJ
{
    /// <summary>
    /// Build a Arc mesh
    /// </summary>
    public class ArcMeshBuilder : SomeMeshBuilder
    {
        public float startDegreeAngle = 0;
        public float endDegreeAngle = 360.0f;
        public float angleStep = 10.0f;
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new ArcMesh(startDegreeAngle, endDegreeAngle, angleStep, worldSize);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
