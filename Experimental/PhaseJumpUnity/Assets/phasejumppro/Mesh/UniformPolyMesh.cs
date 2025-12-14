using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Has unit tests
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Renders a polygon with a uniform distribution of points around its center
    /// (Same thing as a circle, but specified by vertexCount instead of angle
    /// </summary>
    public class UniformPolyMesh : SomeMesh
    {
        public int vertexCount = 3;
        public float radius = 1.0f;

        protected CircleMesh CircleMesh => new CircleMesh(Angle.DegreesAngle(360.0f / vertexCount), radius);

        public UniformPolyMesh(int vertexCount, float radius)
        {
            this.vertexCount = vertexCount;
            this.radius = radius;
        }

        public override Mesh BuildMesh()
        {
            return CircleMesh.BuildMesh();
        }
    }
}
