#pragma once

#include "SomePath.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Simple path
 CODE REVIEW: 4/12/23
 */
namespace PJ {
    /// Model that defines a circle path
    class BezierPath : public SomePath {
        // TODO: Unit tests
    public:
        using Base = SomePath;
        using This = BezierPath;

        VectorList<Vector3> controlPoints;

        BezierPath() {}

        BezierPath(VectorList<Vector3> controlPoints) :
            controlPoints(controlPoints) {}

        This& SetControlPoints(VectorList<Vector3> const& value) {
            this->controlPoints = value;
            return *this;
        }

        Vector3 PositionAt(float progress) override {
            auto t = progress;

            if (controlPoints.size() == 3) {
                return QuadraticValueFor(t, controlPoints[0], controlPoints[1], controlPoints[2]);
            } else if (controlPoints.size() >= 4) {
                return CubicValueFor(
                    t, controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3]
                );
            }

            return Vector3::zero;
        }

        Vector3 QuadraticValueFor(float t, Vector3 p0, Vector3 p1, Vector3 p2) const {
            return p1 + Squared(1.0f - t) * (p0 - p1) + Squared(t) * (p2 - p1);
        }

        Vector3 CubicValueFor(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) const {
            return Cubed(1.0f - t) * p0 + 3 * Squared(1.0f - t) * t * p1 +
                   3 * (1.0f - t) * Squared(t) * p2 + Cubed(t) * p3;
        }

    protected:
        float Squared(float value) const {
            return value * value;
        }

        float Cubed(float value) const {
            return value * value * value;
        }
    };
} // namespace PJ
