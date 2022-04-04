using System;
using UnityEngine;

namespace PJ
{
    public abstract class SomeMesh
    {
        public Mesh mesh;

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

        public virtual Mesh Build(bool rebuild = false)
        {
            if (!rebuild && mesh != null)
            {
                return mesh;
            }

            mesh = new Mesh();

            switch (mesh.indexFormat)
            {
                case UnityEngine.Rendering.IndexFormat.UInt16:
                    if (MeshVertexCount > 65535)
                    {
                        Debug.Log("Error. Vertex Overflow.");
                        return mesh;
                    }
                    break;
                default:
                    break;
            }

            return BuildMesh(mesh);
        }

        public abstract Mesh BuildMesh(Mesh mesh);
        public abstract int MeshVertexCount { get; }

        public void UpdateMesh(Mesh mesh, Vector3[] vertices, int[] triangles, Vector2[] uv)
        {
            mesh.Clear();
            mesh.vertices = vertices;
            mesh.triangles = triangles;
            mesh.uv = uv;

            // Debug Logs:
            //for (int i = 0; i < vertices.Length; i++)
            //{
            //    Debug.Log("Vertex at " + i.ToString() + "is " + vertices[i].ToString());
            //    Debug.Log("UV at " + i.ToString() + "is " + uv[i].ToString());
            //}
            //for (int i = 0; i < triangles.Length; i++)
            //{
            //    Debug.Log("Triangle at " + i.ToString() + "is " + triangles[i].ToString());
            //}

            mesh.RecalculateBounds();
            mesh.RecalculateNormals();
        }
    }
}
