using UnityEngine;
using System;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class CapsuleFrameMesh2D : SomeMesh
    {
        public Vector2 worldSize;
        public Angle angleStep;
        public float strokeWidth;

        public CapsuleFrameMesh2D(Vector2 worldSize, Angle angleStep, float strokeWidth)
        {
            this.worldSize = worldSize;
            this.angleStep = angleStep;
            this.strokeWidth = strokeWidth;
        }

        public float CapRadius => worldSize.y / 2.0f;
        public float InnerWidth => worldSize.x - (CapRadius * 2.0f);

        public override Mesh BuildMesh()
        {
            // Avoid divide by zero
            if (this.angleStep.Degrees == 0) { return new Mesh(); }

            var angleStep = this.angleStep.Degrees;
            var centerPolyMesh = new CenterPolyFrameMesh(strokeWidth);

            var top = worldSize.y * 0.5f * Vector2.up.y;
            var bottom = worldSize.y * 0.5f * Vector2.down.y;

            var topLeftVertex = new Vector3(-worldSize.x * 0.5f + CapRadius, top, 0);
            centerPolyMesh.polygon.vertices.Add(topLeftVertex);
            var topRightVertex = new Vector3(worldSize.x * 0.5f - CapRadius, top);
            centerPolyMesh.polygon.vertices.Add(topRightVertex);

            centerPolyMesh.AddArcVertices(new Vector3(topRightVertex.x, 0), CapRadius, angleStep, 180.0f - angleStep, angleStep);
            centerPolyMesh.polygon.vertices.Add(new Vector3(worldSize.x * 0.5f - CapRadius, bottom, 0));
            centerPolyMesh.polygon.vertices.Add(new Vector3(-worldSize.x * 0.5f + CapRadius, bottom, 0));
            centerPolyMesh.AddArcVertices(new Vector3(topLeftVertex.x, 0), CapRadius, 180.0f + angleStep, 360.0f, angleStep);

            return centerPolyMesh.BuildMesh();
        }
    }
}
