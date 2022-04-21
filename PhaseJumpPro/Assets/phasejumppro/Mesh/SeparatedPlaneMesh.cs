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
    /// A plane mesh where each triangle does not share any vertices with another
    /// Useful for shatter-type animations
    /// </summary>
    public class SeparatedPlaneMesh : SomePlaneMesh
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public Vector2Int VerticesSize
        {
            get
            {
                int vertexXCount = meshSize.x + 1;
                int vertexZCount = meshSize.y + 1;
                return new Vector2Int(vertexXCount, vertexZCount);
            }
        }

        public override int MeshVertexCount
        {
            get
            {
                return meshSize.x * meshSize.y * 6;
            }
        }

        public SeparatedPlaneMesh(Vector2Int meshSize, Vector2 worldSize)
        {
            this.meshSize = meshSize;
            this.worldSize = worldSize;
        }

        public Vector3 MeshVertexFor(Vector2Int position)
        {
            var xValue = (float)position.x / (VerticesSize.x - 1) * worldSize.x - worldSize.x / 2.0f;
            var faceValue = (float)position.y / (VerticesSize.y - 1) * worldSize.y - worldSize.y / 2.0f;
            return new Vector3(xValue, faceValue, 0);
        }

        public Vector2 UVFor(Vector2Int position)
        {
            var xValue = (float)position.x / (VerticesSize.x - 1);
            var yValue = (float)position.y / (VerticesSize.y - 1);
            return new Vector2(xValue, yValue);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            var cellCount = meshSize.x * meshSize.y;
            var trianglesSize = cellCount * 6;
            int verticesSize = trianglesSize;
            var vertices = new Vector3[verticesSize];
            var triangles = new int[trianglesSize];
            var uvSize = verticesSize;
            var uv = new Vector2[uvSize];

            var offset = 0;
            for (int z = 0; z < meshSize.y; z++)
            {
                for (int x = 0; x < meshSize.x; x++)
                {
                    vertices[0 + offset] = MeshVertexFor(new Vector2Int(x, z));
                    vertices[1 + offset] = MeshVertexFor(new Vector2Int(x, z + 1));
                    vertices[2 + offset] = MeshVertexFor(new Vector2Int(x + 1, z));
                    vertices[3 + offset] = MeshVertexFor(new Vector2Int(x, z + 1));
                    vertices[4 + offset] = MeshVertexFor(new Vector2Int(x + 1, z + 1));
                    vertices[5 + offset] = MeshVertexFor(new Vector2Int(x + 1, z));

                    triangles[0 + offset] = 0 + offset;
                    triangles[1 + offset] = 1 + offset;
                    triangles[2 + offset] = 2 + offset;
                    triangles[3 + offset] = 3 + offset;
                    triangles[4 + offset] = 4 + offset;
                    triangles[5 + offset] = 5 + offset;

                    uv[0 + offset] = UVFor(new Vector2Int(x, z));
                    uv[1 + offset] = UVFor(new Vector2Int(x, z + 1));
                    uv[2 + offset] = UVFor(new Vector2Int(x + 1, z));
                    uv[3 + offset] = UVFor(new Vector2Int(x, z + 1));
                    uv[4 + offset] = UVFor(new Vector2Int(x + 1, z + 1));
                    uv[5 + offset] = UVFor(new Vector2Int(x + 1, z));

                    offset += 6;
                }
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
