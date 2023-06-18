using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
PORTED TO: C++
*/
namespace PJ
{
    /// <summary>
    /// Renders an ellipse
    /// </summary>
    public class EllipseMesh : SomeMesh
    {
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public ArcMesh ArcMesh => new ArcMesh(Angle.DegreesAngle(0), Angle.DegreesAngle(360.0f), angleStep, worldSize);

        public EllipseMesh(Angle angleStep, Vector2 worldSize)
        {
            this.angleStep = angleStep;
            this.worldSize = worldSize;
        }

        public override Mesh BuildMesh()
        {
            return ArcMesh.BuildMesh();
        }
    }
}
