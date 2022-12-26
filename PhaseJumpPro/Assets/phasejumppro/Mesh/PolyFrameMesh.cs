using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/4/22
 */
namespace PJ
{
    public class PolyFrameMesh : SomeMesh
    {
        public float strokeWidth = 0.1f;
        public bool isClosed = true;
        public Polygon polygon = new Polygon();

        public int PolyVertexCount
        {
            get
            {
                return polygon.vertices.Count;
            }
        }

        public int SegmentCount
        {
            get
            {
                if (isClosed) {
                    return PolyVertexCount;
                }

                return PolyVertexCount - 1;
            }
        }

        public override int MeshVertexCount
        {
            get
            {
                // Each segment of a polygon must be its own quad, in order for the stroke width to
                // Render at the proper offset along that segment.
                return SegmentCount * 4;
            }
        }

        public PolyFrameMesh(Polygon polygon, bool isClosed, float strokeWidth)
        {
            this.polygon = polygon;
            this.isClosed = isClosed;
            this.strokeWidth = strokeWidth;
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            int meshVertexCount = MeshVertexCount;
            var vertices = new Vector3[meshVertexCount];
            var trianglesSize = SegmentCount * 6;
            var triangles = new int[trianglesSize];
            var uvSize = meshVertexCount;
            var uv = new Vector2[uvSize];

            // Outer frame
            var meshVertexIndex = 0;
            for (int i = 0; i < SegmentCount; i++)
            {
                var vertex = polygon.vertices[i];
                var nextVertex = polygon.vertices[(i + 1) % polygon.vertices.Count];

                var angleVector = new Vector2(nextVertex.x - vertex.x, nextVertex.y - vertex.y);
                //Debug.Log("Angle Vector: " + angleVector.ToString());
                var degreeAngle = new Angle(angleVector);
                //Debug.Log("Degree Angle: " + degreeAngle.ToString());

                var outerOffset = Angle.DegreesAngle(degreeAngle.Degrees - 90.0f).ToVector2(strokeWidth / 2.0f);
                var innerOffset = Angle.DegreesAngle(degreeAngle.Degrees + 90.0f).ToVector2(strokeWidth / 2.0f);

                //Debug.Log("Outer Offset: " + outerOffset.ToString());
                //Debug.Log("Inner Offset: " + innerOffset.ToString());

                // Outer frame
                // FUTURE: support proper stroke in Z-space (only 2D space stroke width is properly supported)
                vertices[meshVertexIndex + 0] = vertex + new Vector3(innerOffset.x, innerOffset.y, 0);
                vertices[meshVertexIndex + 1] = nextVertex + new Vector3(innerOffset.x, innerOffset.y, 0);
                vertices[meshVertexIndex + 2] = vertex + new Vector3(outerOffset.x, outerOffset.y, 0);
                vertices[meshVertexIndex + 3] = nextVertex + new Vector3(outerOffset.x, outerOffset.y, 0);

                meshVertexIndex += 4;
            }

            var center = polygon.Center;
            var size = polygon.Size;
            if (size.x > 0 && size.y > 0)
            {
                for (int i = 0; i < uvSize; i++)
                {
                    var vertex = vertices[i];
                    var distanceFromCenter = vertex - center;
                    var ratio = new Vector2((distanceFromCenter.x + size.x / 2.0f) / size.x, (distanceFromCenter.y + size.y / 2.0f) / size.y);
                    uv[i] = ratio;
                }
            }
            else
            {
                for (int i = 0; i < uvSize; i++)
                {
                    uv[i] = Vector2.zero;
                }
            }

            var t = 0;
            meshVertexIndex = 0;
            for (int i = 0; i < SegmentCount; i++)
            {
                triangles[t] = meshVertexIndex;
                triangles[t + 1] = meshVertexIndex + 2;
                triangles[t + 2] = meshVertexIndex + 1;
                triangles[t + 3] = meshVertexIndex + 2;
                triangles[t + 4] = meshVertexIndex + 3;
                triangles[t + 5] = meshVertexIndex + 1;

                meshVertexIndex += 4;
                t += 6;
            }
            
            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
