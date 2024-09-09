#pragma once

#include "Angle.h"
#include "Polygon.h"

/*
 RATING: 4 stars
 Tested and works (8/25/24). Needs unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Defines shape metrics for centered uniform polygon
    struct CenterPolyShape {
        Angle startAngle;
        Angle endAngle;
        Angle angleStep;

        CenterPolyShape(Angle startAngle, Angle endAngle, Angle angleStep) :
            startAngle(startAngle),
            endAngle(endAngle),
            angleStep(angleStep) {}

        static CenterPolyShape circle;
        static CenterPolyShape hex;
        static CenterPolyShape square;
    };

    /// Builds a centered uniform poly
    class CenterPolyBuilder {
    public:
        Polygon Build(Vector2 center, Vector2 worldSize, CenterPolyShape shape) {
            Polygon poly;

            auto radii = worldSize / 2.0f;

            auto endAngleDegrees = shape.endAngle.Degrees();

            float angleDelta = endAngleDegrees - shape.startAngle.Degrees();
            float angleStep = shape.angleStep.Degrees();

            // Avoid infinite loop
            if (angleStep <= 0 || angleDelta < 0) {
                return poly;
            }

            float angle = shape.startAngle.Degrees();

            while (angle < endAngleDegrees) {
                auto vector = (Vector2)Angle::DegreesAngle(angle);
                auto vertex =
                    Vector3(center.x + vector.x * radii.x, center.y + vector.y * radii.y, 0);
                Add(poly.value, vertex);

                angle += angleStep;
            }

            // Add final step for rounding errors
            if (angle >= endAngleDegrees) {
                auto vector = (Vector2)Angle::DegreesAngle(endAngleDegrees);
                auto vertex =
                    Vector3(center.x + vector.x * radii.x, center.y + vector.y * radii.y, 0);
                Add(poly.value, vertex);
            }

            return poly;
        }
    };
} // namespace PJ
