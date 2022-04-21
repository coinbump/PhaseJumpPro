using System;

namespace PJ
{
    public abstract class SomePolyFrameMesh : SomeMesh
    {
        public abstract int PolyVertexCount { get; }

        public override int MeshVertexCount
        {
            get
            {
                return PolyVertexCount * 2;
            }
        }
    }
}
