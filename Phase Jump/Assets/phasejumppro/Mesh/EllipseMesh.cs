using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 */
namespace PJ
{
    /// <summary>
    /// Renders an ellipse
    /// </summary>
    public class EllipseMesh : SomeMesh
    {
        public float angleStep = 10.0f;
        public Vector2 size = new Vector2(1.0f, 1.0f);

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
                return SliceCount + 1;
            }
        }

        public EllipseMesh(float angleStep, Vector2 size)
        {
            this.angleStep = angleStep;
            this.size = size;
        }

        public Vector3 MeshVertexFor(float degreeAngle)
        {
            var vector = AngleUtils.DegreeAngleToVector2(degreeAngle, 1.0f);
            return new Vector3(vector.x * size.x / 2.0f, vector.y * size.y / 2.0f, 0);
        }

        public Vector2 UVFor(float degreeAngle)
        {
            var vector = AngleUtils.DegreeAngleToVector2(degreeAngle, 1.0f);
            return new Vector2((vector.x + 1.0f) / 2.0f, (vector.y + 1.0f) / 2.0f);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            var vertexCount = MeshVertexCount;
            var sliceCount = SliceCount;
            var trianglesSize = sliceCount * 3;
            int verticesSize = vertexCount;
            var vertices = new Vector3[verticesSize];
            var triangles = new int[trianglesSize];
            var uvSize = verticesSize;
            var uv = new Vector2[uvSize];

            // Center vertex
            vertices[0] = Vector3.zero;
            uv[0] = new Vector2(0.5f, 0.5f);

            // Edge vertices
            for (int i = 0; i < (vertexCount - 1); i++)
            {
                var angle = angleStep * i;
                vertices[i + 1] = MeshVertexFor(angle);
                uv[i + 1] = UVFor(angle);
            }

            var offset = 0;
            for (int i = 0; i < sliceCount; i++)
            {
                triangles[offset] = 0;
                triangles[offset + 1] = i + 1;
                triangles[offset + 2] = (i + 1) % SliceCount + 1;

                offset += 3;
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
