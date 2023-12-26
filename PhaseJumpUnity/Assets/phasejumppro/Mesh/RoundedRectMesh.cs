using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class RoundedRectMesh : SomeMesh
    {
        public Vector2 worldSize;
        public float cornerRadius;
        public Angle angleStep;

        public RoundedRectMesh(Vector2 worldSize, float cornerRadius, Angle angleStep)
        {
            this.worldSize = worldSize;
            this.cornerRadius = cornerRadius;
            this.angleStep = angleStep;
        }

        public float InnerWidth => worldSize.x - (cornerRadius * 2.0f);
        public float InnerHeight => worldSize.y - (cornerRadius * 2.0f);

        public override Mesh BuildMesh()
        {
            var cornerRadius = new Vector2(
                Mathf.Max(0, Mathf.Min(worldSize.x / 2.0f, this.cornerRadius)),
                Mathf.Max(0, Mathf.Min(worldSize.y / 2.0f, this.cornerRadius))
            );

            var corners = new RoundedCorners();
            corners.topLeft = cornerRadius;
            corners.topRight = cornerRadius;
            corners.bottomLeft = cornerRadius;
            corners.bottomRight = cornerRadius;

            var roundedCornersRectMesh = new RoundedCornersRectMesh(worldSize, corners, angleStep);
            return roundedCornersRectMesh.BuildMesh();
        }
    }
}
