using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Specifies x and y radii of corners
    /// </summary>
    [Serializable]
    public struct RoundedCorners
    {
        public Vector2 topLeft;
        public Vector2 topRight;
        public Vector2 bottomLeft;
        public Vector2 bottomRight;
    }

    public class RoundedCornersRectMesh : SomeMesh
    {
        public Vector2 worldSize;
        public RoundedCorners corners;
        public Angle angleStep;

        public RoundedCorners Corners
        {
            get
            {
                var result = corners;
                result.topLeft.x = MathF.Max(0, Mathf.Min(worldSize.x, result.topLeft.x));
                result.topLeft.y = MathF.Max(0, Mathf.Min(worldSize.y, result.topLeft.y));
                result.topRight.x = MathF.Max(0, Mathf.Min(worldSize.x, result.topRight.x));
                result.topRight.y = MathF.Max(0, Mathf.Min(worldSize.y, result.topRight.y));
                result.bottomLeft.x = MathF.Max(0, Mathf.Min(worldSize.x, result.bottomLeft.x));
                result.bottomLeft.y = MathF.Max(0, Mathf.Min(worldSize.y, result.bottomLeft.y));
                result.bottomRight.x = MathF.Max(0, Mathf.Min(worldSize.x, result.bottomRight.x));
                result.bottomRight.y = MathF.Max(0, Mathf.Min(worldSize.y, result.bottomRight.y));
                return result;
            }
        }

        public RoundedCornersRectMesh(Vector2 worldSize, RoundedCorners corners, Angle angleStep)
        {
            this.worldSize = worldSize;
            this.corners = corners;
            this.angleStep = angleStep;
        }

        public override Mesh BuildMesh()
        {
            // Avoid divide by zero
            if (this.angleStep.Degrees == 0) { return new Mesh(); }

            var angleStep = this.angleStep.Degrees;
            var centerPolyMesh = new CenterPolyMesh();

            var left = -worldSize.x * 0.5f;
            var right = worldSize.x * 0.5f;
            var top = worldSize.y * 0.5f * Vector2.up.y;
            var bottom = worldSize.y * 0.5f * Vector2.down.y;

            var corners = Corners;

            // Top edge
            var topLeftVertex = new Vector3(left + corners.topLeft.x, top, 0);
            centerPolyMesh.polygon.vertices.Add(topLeftVertex);
            var topRightVertex = new Vector3(right - corners.topRight.x, top);
            centerPolyMesh.polygon.vertices.Add(topRightVertex);

            // Top-right corner
            centerPolyMesh.AddArcVertices(new Vector2(topRightVertex.x, topRightVertex.y + corners.topRight.y * Vector2.down.y), corners.topRight, angleStep, 90.0f - angleStep, angleStep);

            // Right edge
            centerPolyMesh.polygon.vertices.Add(new Vector3(right, top + corners.topRight.y * Vector2.down.y, 0));
            var bottomRightVertex = new Vector3(right, bottom + corners.bottomRight.y * Vector2.up.y, 0);
            centerPolyMesh.polygon.vertices.Add(bottomRightVertex);

            // Bottom-right corner
            centerPolyMesh.AddArcVertices(new Vector2(right - corners.bottomRight.x, bottom + corners.bottomRight.y * Vector2.up.y), corners.bottomRight, 90.0f + angleStep, 180.0f - angleStep, angleStep);

            // Bottom edge
            centerPolyMesh.polygon.vertices.Add(new Vector3(right - corners.bottomRight.x, bottom, 0));
            var bottomLeftVertex = new Vector3(left + corners.bottomLeft.x, bottom, 0);
            centerPolyMesh.polygon.vertices.Add(bottomLeftVertex);

            // Bottom-left corner
            centerPolyMesh.AddArcVertices(new Vector2(left + corners.bottomLeft.x, bottom + corners.bottomLeft.y * Vector2.up.y), corners.bottomLeft, 180.0f + angleStep, 270.0f - angleStep, angleStep);

            // Left edge
            centerPolyMesh.polygon.vertices.Add(new Vector3(left, bottom + corners.bottomLeft.y * Vector2.up.y, 0));
            var tlVertex = new Vector3(left, top + corners.topLeft.y * Vector2.down.y, 0);
            centerPolyMesh.polygon.vertices.Add(tlVertex);

            // Top-left corner
            centerPolyMesh.AddArcVertices(new Vector2(left + corners.topLeft.x, top + corners.topLeft.y * Vector2.down.y), corners.topLeft, 270.0f + angleStep, 360.0f, angleStep);

            return centerPolyMesh.BuildMesh();
        }
    }
}
