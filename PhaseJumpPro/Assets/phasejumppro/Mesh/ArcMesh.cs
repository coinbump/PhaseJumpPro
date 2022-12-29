using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/*
RATING: 5 stars
Tested and works
CODE REVIEW: 12/28/22
*/
namespace PJ
{
    /// <summary>
    /// Renders an arc
    /// </summary>
    public class ArcMesh : SomeMesh
    {
        public Angle startAngle = Angle.DegreesAngle(0);
        public Angle endAngle = Angle.DegreesAngle(360.0f);
        public Angle angleStep = Angle.DegreesAngle(10.0f);
        public Vector2 worldSize = new Vector2(1.0f, 1.0f);

        public int SliceCount
        {
            get
            {
                if (0 == angleStep.Degrees)
                {
                    // Avoid divide-by-zero
                    return 1;
                }

                return Mathf.CeilToInt((endAngle.Degrees - startAngle.Degrees) / Mathf.Abs(angleStep.Degrees));
            }
        }
        public ArcMesh(Angle startAngle, Angle endAngle, Angle angleStep, Vector2 worldSize)
        {
            this.startAngle = startAngle;
            this.endAngle = endAngle;
            this.angleStep = angleStep;
            this.worldSize = worldSize;
        }

        public override Mesh BuildMesh()
        {
            var centerPolyMesh = new CenterPolyMesh();
            centerPolyMesh.AddArcVertices(Vector2.zero, new Vector2(worldSize.x / 2.0f, worldSize.y / 2.0f), startAngle.Degrees, endAngle.Degrees, angleStep.Degrees);
            return centerPolyMesh.BuildMesh();
        }
    }
}
