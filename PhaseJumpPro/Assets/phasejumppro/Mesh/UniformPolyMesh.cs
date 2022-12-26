using System.Runtime.InteropServices;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/3/22
 */
namespace PJ
{
    /// <summary>
    /// Renders a polygon with a uniform distribution of points around its center
    /// (Same thing as a circle, but with a different parameter type).
    /// </summary>
    public class UniformPolyMesh : SomeMesh
    {
        public int vertexCount = 3;
        public float radius = 1.0f;

        protected CircleMesh circleMesh;

        public override int MeshVertexCount
        {
            get
            {
                return circleMesh.MeshVertexCount;
            }
        }

        public UniformPolyMesh(int vertexCount, float radius)
        {
            this.vertexCount = vertexCount;
            this.radius = radius;
            circleMesh = new CircleMesh(Angle.DegreesAngle(360.0f / vertexCount), radius);
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return circleMesh.BuildMesh(mesh);
        }
    }
}
