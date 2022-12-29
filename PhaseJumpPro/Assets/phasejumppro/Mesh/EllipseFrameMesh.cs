using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Renders an ellipse frame
    /// </summary>
    public class EllipseFrameMesh : SomeMesh
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public float strokeWidth = 0.1f;

        public EllipseFrameMesh(Angle angleStep, Vector2 size, float strokeWidth)
        {
            this.angleStep = angleStep;
            this.worldSize = size;
            this.strokeWidth = strokeWidth;
        }

        public Vector3 MeshVertexFor(Angle angle, Vector2 worldSize)
        {
            var vector = (Vector2)angle;
            return new Vector3(vector.x * worldSize.x / 2.0f, vector.y * worldSize.y / 2.0f, 0);
        }

        public override Mesh BuildMesh()
        {
            var centerPolyMesh = new CenterPolyFrameMesh(strokeWidth);
            centerPolyMesh.AddArcVertices(Vector2.zero, new Vector2(worldSize.x / 2.0f, worldSize.y / 2.0f), 0, 360.0f, angleStep.Degrees);
            // centerPolyMesh.AddArcVertices(Vector2.zero, new Vector2(worldSize.x / 2.0f, worldSize.y / 2.0f), 0, 90.0f, angleStep.Degrees);
            return centerPolyMesh.BuildMesh();
        }
    }
}
