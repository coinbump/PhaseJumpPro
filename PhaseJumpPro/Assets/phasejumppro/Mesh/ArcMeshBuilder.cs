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
    /// Build a Arc mesh
    /// </summary>
    public class ArcMeshBuilder : SomeMeshBuilder
    {
        public Angle startAngle = Angle.DegreesAngle(0);
        public Angle endAngle = Angle.DegreesAngle(360.0f);
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public override Vector2 WorldSize
        {
            get => worldSize;
            set => worldSize = value;
        }

        public override Mesh BuildMesh()
        {
            var arcMesh = new ArcMesh(startAngle, endAngle, angleStep, worldSize);
            return arcMesh.BuildMesh();
        }
    }
}
