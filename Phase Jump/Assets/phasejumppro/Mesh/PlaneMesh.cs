using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    public enum Axis2D
    {
        X, Y
    }

    public enum Axis
    {
        X, Y, Z
    }

    public class PlaneMesh
    {
        public Vector2Int meshSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Axis faceAxis = Axis.Z;

        public Mesh mesh;

        public Vector2Int VerticesSize
        {
            get
            {
                int vertexXCount = meshSize.x + 1;
                int vertexZCount = meshSize.y + 1;
                return new Vector2Int(vertexXCount, vertexZCount);
            }
        }

        public Vector3[] Vertices
        {
            get
            {
                return null != mesh ? mesh.vertices : null;
            }
            set
            {
                if (null != mesh)
                {
                    mesh.vertices = value;
                    mesh.RecalculateBounds();
                }
            }
        }

        public PlaneMesh(Vector2Int meshSize, Vector2 worldSize, Axis faceAxis = Axis.Z)
        {
            this.meshSize = meshSize;
            this.worldSize = worldSize;
            this.faceAxis = faceAxis;
        }

        public Mesh Build(bool rebuild = false)
        {
            if (!rebuild && mesh != null)
            {
                return mesh;
            }

            mesh = new Mesh();

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
                    var faceValue = (float)z / (vertexZCount - 1) * worldSize.y - worldSize.y / 2.0f;
                    switch (faceAxis) {
                        case Axis.Y:
                            vertices[i] = new Vector3((float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, faceValue, 0);
                            break;
                        default:
                            vertices[i] = new Vector3((float)x / (vertexXCount - 1) * worldSize.x - worldSize.x / 2.0f, 0, faceValue);
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
                    triangles[i + 1] = c + vertexXCount;
                    triangles[i + 2] = c + 1;
                    triangles[i + 3] = c + vertexXCount;
                    triangles[i + 4] = c + vertexXCount + 1;
                    triangles[i + 5] = c + 1;
                    i += 6;
                }
            }

            mesh.Clear();
            mesh.vertices = vertices;
            mesh.triangles = triangles;
            mesh.uv = uv;

            // Debug Logs:
            //for (int i = 0; i < verticesSize; i++)
            //{
            //    Debug.Log("Vertex at " + i.ToString() + "is " + vertices[i].ToString());
            //    //Debug.Log("UV at " + i.ToString() + "is " + uv[i].ToString());
            //}
            //for (int i = 0; i < trianglesSize; i++)
            //{
            //    Debug.Log("Triangle at " + i.ToString() + "is " + triangles[i].ToString());
            //}

            mesh.RecalculateBounds();
            mesh.RecalculateNormals();

            return mesh;
        }
    }
}
