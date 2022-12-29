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
    /// Build a Ellipse frame mesh
    /// </summary>
    public class EllipseFrameMeshBuilder : SomeMeshBuilder
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public float strokeWidth = 0.1f;

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var shapeMesh = new EllipseFrameMesh(angleStep, worldSize, strokeWidth);
            var mesh = shapeMesh.Build(true);
            return mesh;
        }
    }
}
