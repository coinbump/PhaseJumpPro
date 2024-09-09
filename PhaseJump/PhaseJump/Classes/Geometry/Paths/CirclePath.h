#pragma once

#include "Angle.h"
#include "SomePath.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Model that defines a circle path
    class CirclePath : public SomePath {
    public:
        float radius;

        CirclePath(float radius) :
            radius(radius) {}

        Vector3 PositionAt(float progress) override {
            auto result = Angle::DegreesAngle(progress * 360.0f).ToVector2(radius);
            return Vector3(result.x, result.y, 0);
        }
    };

    // FUTURE: support rotated circles in 3D space if needed
} // namespace PJ
