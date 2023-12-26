using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class Capsule2DMeshBuilder : SomeMeshBuilder
    {
        public Vector2 worldSize = new Vector2(1.0f, 0.5f);
        public Angle angleStep = Angle.DegreesAngle(10.0f);

        protected override Vector2 WorldSizeInternal
        {
            get => worldSize;
            set
            {
                worldSize = value;
            }
        }

        public override Mesh BuildMesh()
        {
            return new Capsule2DMesh(worldSize, angleStep).Build(true);
        }
    }
}
