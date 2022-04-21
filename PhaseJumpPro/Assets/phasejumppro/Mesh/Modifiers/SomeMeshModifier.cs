using System;
using UnityEngine;

namespace PJ
{
	/// <summary>
    /// Modify a mesh and return the modified mesh
    /// </summary>
	public abstract class SomeMeshModifier
	{
		public abstract Mesh ModifyMesh(Mesh mesh);
	}

    /// <summary>
    /// Reflect the UV coordinates horizontally
    /// Example: 0 .5 1 1 .5 0
    /// </summary>
    public class ReflectUVXMeshModifier : SomeMeshModifier
    {
        public override Mesh ModifyMesh(Mesh mesh)
        {
            if (mesh.uv.Length < mesh.vertices.Length)
            {
                Debug.Log("Error. Missing UVs for modifier");
                return mesh;
            }

            var meshSize = mesh.bounds.size;
            for (int i = 0; i < mesh.vertices.Length; i++)
            {

            }

            return mesh;
        }
    }
}
