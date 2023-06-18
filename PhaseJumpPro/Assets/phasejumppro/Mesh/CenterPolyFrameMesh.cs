using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 4 stars
Tested and works, but there might be a more optimal algorithm
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// A frame mesh that can be defined by a center point at (0, 0) and a surrounding path
    /// Example: capsule, rounded rectangle, arc, ellipse, circle
    /// </summary>
    public class CenterPolyFrameMesh : SomeCenterPolyMesh
    {
        public float strokeWidth;

        public int MeshVertexCount => (polygon.Count) * 2;
        public int SliceCount => polygon.Count - 1;

        public CenterPolyFrameMesh(float strokeWidth)
        {
            this.strokeWidth = strokeWidth;
        }

        public bool IsClosedShape => polygon[0] == polygon[polygon.Count - 1];

        public override Mesh BuildMesh()
        {
            var result = new Mesh();
            if (polygon.Count < 2) { return result; }

            var vertexCount = MeshVertexCount;
            var vertices = new Vector3[vertexCount];
            var uvCount = vertexCount;
            var uvs = new Vector2[uvCount];
            var sliceCount = SliceCount;
            var trianglesCount = sliceCount * 6;
            var triangles = new int[trianglesCount];

            var polyWithCenter = new Polygon();
            polyWithCenter.Add(Vector3.zero);
            polyWithCenter.AddRange(polygon);
            var polygonMin = polyWithCenter.Min;
            var polygonSize = polyWithCenter.Size;

            // Edge vertices
            int vi = 0;
            foreach (var vertex in polygon)
            {
                vertices[vi] = vertex;
                uvs[vi] = new Vector2((vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y);
                vi++;
            }

            var firstVi = vi;
            Vector3 prevVertex = Vector3.zero;
            bool hasPrevVertex = false;
            int index = 0;

            // This has only been tested with closed shapes
            foreach (var vertex in polygon)
            {
                var thisVertex = vertex;

                Vector3 reference = vertex;
                if (IsClosedShape)
                {
                    reference = polygon[polygon.Count - 2];
                }
                if (hasPrevVertex)
                {
                    reference = prevVertex;
                }

                prevVertex = vertex;
                hasPrevVertex = true;

                var lineVector = vertex - reference;
                var lineAngle = new Angle(lineVector);

                var nextLineVertex = polygon[polygon.Count - 1];
                if (index + 1 < polygon.Count)
                {
                    nextLineVertex = polygon[index + 1];
                }

                var nextLineVector = nextLineVertex - vertex;
                var nextLineAngle = new Angle(nextLineVector);

                var innerOffset = Angle.DegreesAngle(lineAngle.Degrees + 90.0f).ToVector2(strokeWidth) / 2.0f + Angle.DegreesAngle(nextLineAngle.Degrees + 90.0f).ToVector2(strokeWidth) / 2.0f;
                thisVertex += new Vector3(innerOffset.x, innerOffset.y, 0);

                vertices[vi] = thisVertex;
                uvs[vi] = new Vector2((thisVertex.x - polygonMin.x) / polygonSize.x, (thisVertex.y - polygonMin.y) / polygonSize.y);
                vi++;
                index++;
            }

            // Final cleanup for closed shape
            if (IsClosedShape)
            {
                vertices[vi - 1] = vertices[firstVi];
            }

            var offset = 0;
            for (int i = 0; i < sliceCount; i++)
            {
                triangles[offset] = i;
                triangles[offset + 1] = i + 1;
                triangles[offset + 2] = i + polygon.Count;

                triangles[offset + 3] = i + 1;
                triangles[offset + 4] = i + polygon.Count + 1;
                triangles[offset + 5] = i + polygon.Count;

                offset += 6;
            }

            UpdateMesh(result, vertices, triangles, uvs);

            return result;
        }
    }
}
