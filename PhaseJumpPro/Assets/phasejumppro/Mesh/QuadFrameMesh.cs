using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 */
namespace PJ
{
    public class QuadFrameMesh : SomePolyFrameMesh
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public Vector2 strokeSize = new Vector2(0.1f, 0.1f);

        public override int PolyVertexCount
        {
            get
            {
                return 4;
            }
        }

        public QuadFrameMesh(Vector2 worldSize, Vector2 strokeSize)
        {
            this.worldSize = worldSize;
            this.strokeSize = strokeSize;
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            int meshVertexCount = MeshVertexCount;
            var vertices = new Vector3[meshVertexCount];
            var trianglesSize = MeshVertexCount * 3;
            var triangles = new int[trianglesSize];
            var uvSize = meshVertexCount;
            var uv = new Vector2[uvSize];

            // Outer frame
            vertices[0] = new Vector3(-worldSize.x / 2.0f, worldSize.y / 2.0f, 0);
            vertices[1] = new Vector3(worldSize.x / 2.0f, worldSize.y / 2.0f, 0);
            vertices[2] = new Vector3(worldSize.x / 2.0f, -worldSize.y / 2.0f, 0);
            vertices[3] = new Vector3(-worldSize.x / 2.0f, -worldSize.y / 2.0f, 0);
            uv[0] = new Vector2(0, 1);
            uv[1] = new Vector2(1, 1);
            uv[2] = new Vector2(1, 0);
            uv[3] = new Vector2(0, 0);

            // Inner frame
            var frameSize = new Vector2(Mathf.Abs(worldSize.x - strokeSize.x * 2.0f), Mathf.Abs(worldSize.y - strokeSize.y * 2.0f));
            vertices[4] = new Vector3(-frameSize.x / 2.0f, frameSize.y / 2.0f, 0);
            vertices[5] = new Vector3(frameSize.x / 2.0f, frameSize.y / 2.0f, 0);
            vertices[6] = new Vector3(frameSize.x / 2.0f, -frameSize.y / 2.0f, 0);
            vertices[7] = new Vector3(-frameSize.x / 2.0f, -frameSize.y / 2.0f, 0);

            var frameRatioX = strokeSize.x / worldSize.x;
            var frameRatioY = strokeSize.y / worldSize.y;
            uv[4] = new Vector2(frameRatioX, 1.0f - frameRatioY);
            uv[5] = new Vector2(1.0f - frameRatioX, 1.0f - frameRatioY);
            uv[6] = new Vector2(1.0f - frameRatioX, frameRatioY);
            uv[7] = new Vector2(frameRatioX, frameRatioY);

            var t = 0;
            var polyVertexCount = PolyVertexCount;

            for (int i = 0; i < polyVertexCount; i++)
            {
                int nextInnerVertexIndex = ((i + polyVertexCount + 1) - polyVertexCount) % polyVertexCount + polyVertexCount;

                triangles[t] = i;
                triangles[t + 1] = nextInnerVertexIndex;
                triangles[t + 2] = i + polyVertexCount;
                triangles[t + 3] = i;

                triangles[t + 4] = (i + 1) % polyVertexCount;   // Wrap to first vertex on last triangle
                triangles[t + 5] = nextInnerVertexIndex;

                t += 6;
            }

            UpdateMesh(mesh, vertices, triangles, uv);

            return mesh;
        }
    }
}
