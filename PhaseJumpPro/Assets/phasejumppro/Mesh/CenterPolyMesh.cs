using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// A mesh that can be defined by a center point at (0, 0) and a surrounding path
    /// Example: capsule, rounded rectangle, arc, ellipse, circle
    /// </summary>
    public class CenterPolyMesh : SomeCenterPolyMesh
    {
        public int MeshVertexCount => polygon.Count + 1;
        public int SliceCount => polygon.Count - 1;

        public CenterPolyMesh() { }

        public override Mesh BuildMesh()
        {
            var mesh = new Mesh();
            if (polygon.Count < 2) { return mesh; }

            var vertexCount = MeshVertexCount;
            var vertices = new Vector3[vertexCount];
            var uvCount = vertexCount;
            var uv = new Vector2[uvCount];
            var sliceCount = SliceCount;
            var trianglesCount = sliceCount * 3;
            var triangles = new int[trianglesCount];

            // Center vertex
            vertices[0] = Vector3.zero;
            uv[0] = new Vector2(0.5f, 0.5f);

            var polyWithCenter = new Polygon();
            polyWithCenter.Add(vertices[0]);
            polyWithCenter.AddRange(polygon);
            var polygonMin = polyWithCenter.Min;
            var polygonSize = polyWithCenter.Size;

            // Edge vertices
            int vi = 1;
            foreach (var vertex in polygon)
            {
                vertices[vi] = vertex;
                uv[vi] = new Vector2((vertex.x - polygonMin.x) / polygonSize.x, (vertex.y - polygonMin.y) / polygonSize.y);
                vi++;
            }

            var offset = 0;
            for (int i = 0; i < sliceCount; i++)
            {
                triangles[offset] = 0;
                triangles[offset + 1] = i + 1;
                triangles[offset + 2] = i + 2;

                offset += 3;
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
