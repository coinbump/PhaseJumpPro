#pragma once

#include "Angle.h"
#include "Polygon.h"

/*
 RATING: 4 stars
 Has unit tests
 CODE REVIEW: 12/14/24
 */
namespace PJ {
    /// Defines shape metrics for centered uniform polygon
    struct CenterPolyModel {
        Angle startAngle;

        /// Distance from start angle to end angle
        Angle angleDelta = Angle::WithDegrees(360);

        Angle angleStep = Angle::WithDegrees(10);

        static CenterPolyModel ellipse;
        static CenterPolyModel hex;
        static CenterPolyModel quad;

        bool IsClosed() const {
            return angleDelta.Degrees() >= 360.0f;
        }
    };

    /// Builds a centered uniform poly
    class CenterPolyBuilder {
    public:
        Polygon Build(Vector2 center, Vector2 worldSize, CenterPolyModel shape);
    };
} // namespace PJ
