using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/4/22
 */
namespace PJ
{
    /// <summary>
    /// Renders an ellipse frame
    /// </summary>
    public class EllipseFrameMesh : SomeMesh
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public float strokeWidth = 0.1f;

        private PolyFrameMesh polyFrameMesh;

        public int SliceCount
        {
            get
            {
                if (0 == angleStep.Degrees)
                {
                    // Avoid divide-by-zero
                    return 1;
                }

                return (int)(360.0f / Mathf.Abs(angleStep.Degrees));
            }
        }

        public override int MeshVertexCount
        {
            get
            {
                // Center + slice vertices
                return polyFrameMesh.MeshVertexCount;
            }
        }

        public EllipseFrameMesh(Angle angleStep, Vector2 size, float strokeWidth)
        {
            this.angleStep = angleStep;
            this.worldSize = size;
            this.strokeWidth = strokeWidth;

            var polygon = new Polygon();
            for (int i = 0; i < SliceCount; i++)
            {
                polygon.vertices.Add(MeshVertexFor(Angle.DegreesAngle(angleStep.Degrees * i)));
            }
            polyFrameMesh = new PolyFrameMesh(polygon, true, strokeWidth);
        }

        public Vector3 MeshVertexFor(Angle angle)
        {
            var vector = (Vector2)angle;
            return new Vector3(vector.x * worldSize.x / 2.0f, vector.y * worldSize.y / 2.0f, 0);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return polyFrameMesh.BuildMesh(mesh);
        }
    }
}
