#pragma once

#include "BezierPath.h"
#include "SomePathLayout.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: rethink layouts using composition
    /// Distribute objects along a circle path
    class BezierPathLayout : public SomePathLayout {
    public:
        VectorList<Vector3> controlPoints;

        BezierPathLayout& SetControlPoints(VectorList<Vector3> const& value) {
            controlPoints = value;

            ApplyLayout();

            return *this;
        }

        BezierPathLayout& SetControlPoint(int i, Vector3 value) {
            GUARDR(i >= 0 && i < controlPoints.size(), *this)
            controlPoints[i] = value;

            ApplyLayout();

            return *this;
        }

        // MARK: SomePathLayout

        SP<SomePath> BuildPath() override {
            return SCAST<SomePath>(MAKE<BezierPath>(controlPoints));
        }
    };
} // namespace PJ
