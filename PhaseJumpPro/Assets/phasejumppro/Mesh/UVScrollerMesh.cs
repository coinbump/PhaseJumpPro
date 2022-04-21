using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/15/22
 */
namespace PJ
{
    /// <summary>
    /// A UV Scroller mesh has two quads. The quads are moved along with the UV coordinates
    /// to simulate endless horizontal scrolling for a repeating texture
    /// </summary>
    public class UVScrollerMesh : SomeMesh
    {
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);
        public float scrollValue;

        public override int MeshVertexCount => 8;

        protected Vector3[] vertices = new Vector3[8];
        protected Vector2[] uv = new Vector2[8];

        public float ScrollValue
        {
            get => scrollValue;
            set
            {
                if (scrollValue != value)
                {
                    scrollValue = value;
                    if (null == mesh)
                    {
                        Build();
                    }
                    else
                    {
                        BuildVerticesAndUVs(mesh);
                    }
                }
            }
        }

        public UVScrollerMesh(Vector2 worldSize, float scrollValue)
        {
            this.worldSize = worldSize;
            this.scrollValue = scrollValue;
        }

        public void BuildVerticesAndUVs(Mesh mesh)
        {
            // Bottom row
            vertices[0] = new Vector3(-worldSize.x / 2.0f, (worldSize.y / 2.0f) * Vector2.down.y, 0);
            vertices[1] = new Vector3(vertices[0].x + worldSize.x * (1.0f - scrollValue), vertices[0].y, 0);
            vertices[2] = new Vector3(vertices[1].x, vertices[0].y, 0);
            vertices[3] = new Vector3(vertices[0].x + worldSize.x, vertices[0].y, 0);

            // Top row
            vertices[4] = new Vector3(vertices[0].x, (worldSize.y / 2.0f) * Vector2.up.y, 0);
            vertices[5] = new Vector3(vertices[1].x, vertices[4].y, 0);
            vertices[6] = new Vector3(vertices[2].x, vertices[4].y, 0);
            vertices[7] = new Vector3(vertices[3].x, vertices[4].y, 0);

            uv[0] = new Vector2(scrollValue, 0);
            uv[1] = new Vector2(1.0f, 0);
            uv[2] = new Vector2(0, 0);
            uv[3] = new Vector2(scrollValue, 0);

            uv[4] = new Vector2(uv[0].x, 1);
            uv[5] = new Vector2(uv[1].x, 1);
            uv[6] = new Vector2(uv[2].x, 1);
            uv[7] = new Vector2(uv[3].x, 1);

            mesh.vertices = vertices;
            mesh.uv = uv;
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            BuildVerticesAndUVs(mesh);

            var triangles = new int[12];
            triangles[0] = 0;
            triangles[1] = 4;
            triangles[2] = 1;
            triangles[3] = 4;
            triangles[4] = 5;
            triangles[5] = 1;

            triangles[6] = 2;
            triangles[7] = 6;
            triangles[8] = 3;
            triangles[9] = 6;
            triangles[10] = 7;
            triangles[11] = 3;

            mesh.triangles = triangles;

            return mesh;
        }
    }
}
