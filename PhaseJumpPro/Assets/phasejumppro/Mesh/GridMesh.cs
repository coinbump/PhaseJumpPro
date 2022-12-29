using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    public class GridMesh : SomeMesh
    {
        public Vector2Int gridSize = new Vector2Int(3, 3);
        public Vector2 worldSize = new Vector2(3.0f, 3.0f);
        public Vector2 cellSize = new Vector2(1.0f, 1.0f);  // Size 0 means: "auto calculate cell size"
        public float strokeWidth = 0.1f;

        public int VerticalLinesCount
        {
            get
            {
                return gridSize.x - 1;
            }
        }

        public int HorizontalLinesCount
        {
            get
            {
                return gridSize.y - 1;
            }
        }

        public int SegmentCount
        {
            get
            {
                return VerticalLinesCount + HorizontalLinesCount;
            }
        }

        public int MeshVertexCount
        {
            get
            {
                // Each line is its own quad
                return SegmentCount * 4;
            }
        }

        public GridMesh(Vector2Int gridSize, Vector2 worldSize, Vector2 cellSize, float strokeWidth)
        {
            this.gridSize = gridSize;
            this.worldSize = worldSize;
            this.cellSize = cellSize;
            this.strokeWidth = strokeWidth;
        }

        public override Mesh BuildMesh()
        {
            var mesh = new Mesh();

            int meshVertexCount = MeshVertexCount;
            var vertices = new Vector3[meshVertexCount];
            var trianglesSize = SegmentCount * 6;
            var triangles = new int[trianglesSize];
            var uvSize = meshVertexCount;
            var uv = new Vector2[uvSize];

            var cellSize = this.cellSize;
            if (cellSize.x == 0)
            {
                cellSize.x = worldSize.x / gridSize.x;
            }
            if (cellSize.y == 0)
            {
                cellSize.y = worldSize.y / gridSize.y;
            }

            var viewBounds = new Rect(new Vector2(-worldSize.x / 2.0f, (worldSize.y / 2.0f) * Vector2.up.y), worldSize);

            // Vertical lines
            var meshVertexIndex = 0;
            var linesWidth = (VerticalLinesCount - 1) * cellSize.x;
            for (int x = 0; x < VerticalLinesCount; x++)
            {
                var xPosition = -(linesWidth / 2.0f) + x * cellSize.x;
                var xLeft = xPosition - strokeWidth / 2.0f;
                var xRight = xPosition + strokeWidth / 2.0f;
                var yTop = (worldSize.y / 2.0f) * Vector2.up.y;
                var yBottom = (worldSize.y / 2.0f) * Vector2.down.y;

                vertices[meshVertexIndex + 0] = new Vector3(xLeft, yTop, 0);
                vertices[meshVertexIndex + 1] = new Vector3(xLeft, yBottom, 0);
                vertices[meshVertexIndex + 2] = new Vector3(xRight, yTop, 0);
                vertices[meshVertexIndex + 3] = new Vector3(xRight, yBottom, 0);

                meshVertexIndex += 4;
            }

            // Horizontal lines
            var linesHeight = (HorizontalLinesCount - 1) * cellSize.y;
            for (int y = 0; y < HorizontalLinesCount; y++)
            {
                var yPosition = (linesHeight / 2.0f) * Vector2.up.y + y * cellSize.y * Vector2.down.y;
                var yTop = yPosition + (strokeWidth / 2.0f) * Vector2.up.y;
                var yBottom = yPosition + (strokeWidth / 2.0f) * Vector2.down.y;
                var xLeft = -(worldSize.x / 2.0f);
                var xRight = (worldSize.x / 2.0f);

                vertices[meshVertexIndex + 0] = new Vector3(xLeft, yBottom, 0);
                vertices[meshVertexIndex + 1] = new Vector3(xRight, yBottom, 0);
                vertices[meshVertexIndex + 2] = new Vector3(xLeft, yTop, 0);
                vertices[meshVertexIndex + 3] = new Vector3(xRight, yTop, 0);

                meshVertexIndex += 4;
            }

            for (int i = 0; i < meshVertexCount; i++)
            {
                var meshVertex = vertices[i];
                uv[i] = new Vector2((meshVertex.x - viewBounds.position.x) / worldSize.x, Mathf.Abs(meshVertex.y - viewBounds.position.y) / worldSize.y);
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
