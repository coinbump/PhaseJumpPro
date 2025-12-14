using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class CapsuleMeshBuilder2D : SomeMeshBuilder
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
            return new CapsuleMesh2D(worldSize, angleStep).Build(true);
        }
    }
}
