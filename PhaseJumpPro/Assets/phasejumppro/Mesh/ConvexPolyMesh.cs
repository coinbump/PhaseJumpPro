using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple mesh
 * CODE REVIEW: 4/7/22
 */
namespace PJ
{
    /// <summary>
    /// Renders an ConvexPoly (no concave turns or holes)
    /// Works for clockwise-turning polygons
    /// </summary>
    public class ConvexPolyMesh : SomeMesh
    {
        public Polygon polygon;
        
        public int TriangleCount
        {
            get
            {
                return polygon.vertices.Count - 2;
            }
        }

        public override int MeshVertexCount
        {
            get
            {
                return polygon.vertices.Count;
            }
        }

        public ConvexPolyMesh(Polygon polygon)
        {
            this.polygon = polygon;
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            if (TriangleCount < 1)
            {
                Debug.Log("Error. Not enough polygon vertices for Convex Poly Mesh");
                return mesh;
            }

            var vertexCount = MeshVertexCount;
            var triangleCount = TriangleCount;
            var trianglesSize = triangleCount * 3;
            int verticesSize = vertexCount;
            var vertices = new Vector3[verticesSize];
            var triangles = new int[trianglesSize];
            var uvSize = verticesSize;
            var uv = new Vector2[uvSize];

            // FUTURE: support sorting counterclockwise polygons to clockwise if needed
            var polygonMin = polygon.Min;
            var polygonSize = polygon.Size;

            for (int i = 0; i < vertexCount; i++)
            {
                vertices[i] = polygon.vertices[i];
                uv[i] = new Vector2((vertices[i].x - polygonMin.x) / polygonSize.x, (vertices[i].y - polygonMin.y) / polygonSize.y);
            }

            // Build triangle fan with shared vertices
            var t = 0;
            var nextVertex = 1;
            for (int i = 0; i < TriangleCount; i++) {
                triangles[t + 0] = 0;
                triangles[t + 1] = nextVertex;
                triangles[t + 2] = nextVertex + 1;

                t += 3;
                nextVertex += 1;
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
