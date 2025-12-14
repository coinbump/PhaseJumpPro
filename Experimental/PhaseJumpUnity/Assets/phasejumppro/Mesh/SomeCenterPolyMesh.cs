using UnityEngine;
using System;

/*
RATING: 4 stars
Tested and works. Needs unit tests
CODE REVIEW: 4/16/23
PORTED TO: C++
*/
namespace PJ
{
    public abstract class SomeCenterPolyMesh : SomeMesh
    {
        public Polygon polygon = new();

        public void AddArcVertices(Vector2 center, Vector2 radius, float startAngle, float endAngle, float angleStep)
        {
            float angleDelta = endAngle - startAngle;

            // Avoid infinite loop
            if (angleStep <= 0 || angleDelta < 0) { return; }

            float angle = startAngle;

            while (angle < endAngle)
            {
                var vector = (Vector2)Angle.DegreesAngle(angle);
                var vertex = new Vector3(center.x + vector.x * radius.x, center.y + vector.y * radius.y, 0);
                polygon.Add(vertex);

                angle += angleStep;
            }

            // Add final step for rounding errors
            if (angle >= endAngle)
            {
                var vector = (Vector2)Angle.DegreesAngle(endAngle);
                var vertex = new Vector3(center.x + vector.x * radius.x, center.y + vector.y * radius.y, 0);
                polygon.Add(vertex);
            }
        }

        public void AddArcVertices(Vector2 center, float radius, float startAngle, float endAngle, float angleStep)
        {
            AddArcVertices(center, new Vector2(radius, radius), startAngle, endAngle, angleStep);
        }
    }
}
