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
}
