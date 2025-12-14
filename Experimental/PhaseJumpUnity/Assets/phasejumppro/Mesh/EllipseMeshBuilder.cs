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
    /// Build a Ellipse mesh
    /// </summary>
    public class EllipseMeshBuilder : SomeMeshBuilder
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Angle angleStep = Angle.DegreesAngle(10.0f);

        protected override Vector2 WorldSizeInternal
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
