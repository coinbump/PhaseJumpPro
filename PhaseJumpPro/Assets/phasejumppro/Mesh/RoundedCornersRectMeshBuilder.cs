using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class RoundedCornersRectMeshBuilder : SomeMeshBuilder
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public RoundedCorners corners = new();
        public Angle angleStep = Angle.DegreesAngle(10.0f);

        public override Vector2 WorldSize
        {
            get => worldSize;
            set
            {
                worldSize = value;
            }
        }

        public override Mesh BuildMesh()
        {
            return new RoundedCornersRectMesh(worldSize, corners, angleStep).Build(true);
        }
    }
}
