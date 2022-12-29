using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class PlaneMesh : SomePlaneMesh
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;

        public Vector2Int VerticesSize
        {
            get
            {
                int vertexXCount = meshSize.x + 1;
                int vertexZCount = meshSize.y + 1;
                return new Vector2Int(vertexXCount, vertexZCount);
            }
        }

        public int MeshVertexCount
        {
            get
            {
                return VerticesSize.x * VerticesSize.y;
            }
        }

        public PlaneMesh(Vector2Int meshSize, Vector2 worldSize, Axis faceAxis = Axis.Z)
        {
            this.meshSize = meshSize;
            this.worldSize = worldSize;
            this.faceAxis = faceAxis;
        }

        public override Mesh BuildMesh()
        {
            var mesh = new Mesh();

            int vertexXCount = meshSize.x + 1;
            int vertexZCount = meshSize.y + 1;
            int verticesSize = vertexXCount * vertexZCount;
            var vertices = new Vector3[verticesSize];
            var cellCount = meshSize.x * meshSize.y;
            var trianglesSize = cellCount * 6;
            var triangles = new int[trianglesSize];
            var uvSize = verticesSize;
            var uv = new Vector2[uvSize];

            for (int z = 0, i = 0; z < vertexZCount; z++)
            {
                for (int x = 0; x < vertexXCount; x++)
                {
                    // Y, Z axes supported (for now)
                    switch (faceAxis)
                    {
                        case Axis.Y:
                            {
                                var faceValue = worldSize.y * 0.5f * Vector2.up.y + ((float)z / (vertexZCount - 1)) * worldSize.y * Vector2.down.y;
                                vertices[i] = new Vector3((float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, faceValue, 0);
                            }
                            break;
                        default:
                            {
                                var faceValue = worldSize.y * 0.5f * Vector3.forward.z + (float)z / (vertexZCount - 1) * worldSize.y * Vector3.back.z;
                                vertices[i] = new Vector3((float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, 0, faceValue);
                            }
                            break;
                    }

                    uv[i] = new Vector2((float)x / (float)(vertexXCount - 1), (float)z / (float)(vertexZCount - 1));
                    i++;
                }
            }

            for (int z = 0, i = 0; z < meshSize.y; z++)
            {
                for (int x = 0; x < meshSize.x; x++)
                {
                    var c = z * vertexXCount + x;

                    triangles[i] = c;
                    triangles[i + 1] = c + 1;
                    triangles[i + 2] = c + vertexXCount;
                    triangles[i + 3] = c + 1;
                    triangles[i + 4] = c + vertexXCount + 1;
                    triangles[i + 5] = c + vertexXCount;
                    i += 6;
                }
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
