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
    /// Renders a circle
    /// </summary>
    public class CircleMesh : SomeMesh
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public float radius = 1.0f;

        protected EllipseMesh ellipseMesh;

        public override int MeshVertexCount
        {
            get {
                return ellipseMesh.MeshVertexCount;
            }
        }

        public CircleMesh(Angle angleStep, float radius)
        {
            this.angleStep = angleStep;
            this.radius = radius;
            ellipseMesh = new EllipseMesh(angleStep, new Vector2(radius * 2.0f, radius * 2.0f));
        }

        public override Mesh BuildMesh(Mesh mesh)
        {
            return ellipseMesh.BuildMesh(mesh);
        }
    }
}
