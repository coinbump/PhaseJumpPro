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
    /// Renders a circle
    /// </summary>
    public class CircleMesh : SomeMesh
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public float radius = 1.0f;

        protected EllipseMesh EllipseMesh => new EllipseMesh(angleStep, new Vector2(radius * 2.0f, radius * 2.0f));

        public CircleMesh(Angle angleStep, float radius)
        {
            this.angleStep = angleStep;
            this.radius = radius;
        }

        public override Mesh BuildMesh()
        {
            return EllipseMesh.BuildMesh();
        }
    }
}
