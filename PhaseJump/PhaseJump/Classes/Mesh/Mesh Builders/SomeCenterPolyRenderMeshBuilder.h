#ifndef PJSOMECENTERPOLYMESHRENDERBUILDER_H
#define PJSOMECENTERPOLYMESHRENDERBUILDER_H

#include "Angle.h"
#include "Polygon.h"
#include "SomeRenderMeshBuilder.h"

/*
 RATING: 4 stars
 Tested and works. Needs unit tests
 CODE REVIEW: 4/16/23
 */
namespace PJ
{
    class SomeCenterPolyRenderMeshBuilder : public SomeRenderMeshBuilder
    {
    public:
        Polygon polygon;

        void AddArcVertices(Vector2 center, Vector2 radius, float startAngle, float endAngle, float angleStep)
        {
            float angleDelta = endAngle - startAngle;

            // Avoid infinite loop
            if (angleStep <= 0 || angleDelta < 0) { return; }

            float angle = startAngle;

            while (angle < endAngle)
            {
                auto vector = (Vector2)Angle::DegreesAngle(angle);
                auto vertex = Vector3(center.x + vector.x * radius.x, center.y + vector.y * radius.y, 0);
                polygon.Add(vertex);

                angle += angleStep;
            }

            // Add final step for rounding errors
            if (angle >= endAngle)
            {
                auto vector = (Vector2)Angle::DegreesAngle(endAngle);
                auto vertex = Vector3(center.x + vector.x * radius.x, center.y + vector.y * radius.y, 0);
                polygon.Add(vertex);
            }
        }

        void AddArcVertices(Vector2 center, float radius, float startAngle, float endAngle, float angleStep)
        {
            AddArcVertices(center, Vector2(radius, radius), startAngle, endAngle, angleStep);
        }
    };
}

#endif
