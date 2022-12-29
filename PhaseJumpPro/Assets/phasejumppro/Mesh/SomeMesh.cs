using System;
using System.Collections.Generic;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Logic to build a mesh (we can't subclass Mesh)
    /// Usage: Unity uses clockwise winding order to determine front faces
    /// </summary>
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

            mesh = BuildMesh();

#if DEBUG
            switch (mesh.indexFormat)
            {
                case UnityEngine.Rendering.IndexFormat.UInt16:
                    if (new List<Vector3>(mesh.vertices).Count > 65535)
                    {
                        Debug.Log("Error. Vertex Overflow.");
                        return mesh;
                    }
                    break;
                default:
                    break;
            }
#endif
            return mesh;
        }

        public abstract Mesh BuildMesh();

        public void UpdateMesh(Mesh mesh, Vector3[] vertices, int[] triangles, Vector2[] uv)
        {
            mesh.vertices = vertices;

            if (null != triangles)
            {
                mesh.triangles = triangles;
            }

            if (null != uv)
            {
                mesh.uv = uv;
            }

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
