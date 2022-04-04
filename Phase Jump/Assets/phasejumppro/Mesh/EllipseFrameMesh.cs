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
        public float angleStep = 10.0f;
        public Vector2 size = new Vector2(1.0f, 1.0f);
        public float strokeWidth = 0.1f;

        private PolyFrameMesh polyFrameMesh;

        public int SliceCount
        {
            get
            {
                if (0 == angleStep)
                {
                    // Avoid divide-by-zero
                    return 1;
                }

                return (int)(360.0f / Mathf.Abs(angleStep));
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

        public EllipseFrameMesh(float angleStep, Vector2 size, float strokeWidth)
        {
            this.angleStep = angleStep;
            this.size = size;
            this.strokeWidth = strokeWidth;

            var polygon = new Polygon();
            for (int i = 0; i < SliceCount; i++)
            {
                polygon.vertices.Add(MeshVertexFor(angleStep * i));
            }
            polyFrameMesh = new PolyFrameMesh(polygon, true, strokeWidth);
        }

        public Vector3 MeshVertexFor(float degreeAngle)
        {
            var vector = AngleUtils.DegreeAngleToVector2(degreeAngle, 1.0f);
            return new Vector3(vector.x * size.x / 2.0f, vector.y * size.y / 2.0f, 0);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return polyFrameMesh.BuildMesh(mesh);
        }
    }
}
