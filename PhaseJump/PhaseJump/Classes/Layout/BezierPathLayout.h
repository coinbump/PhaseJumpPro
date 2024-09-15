#pragma once

#include "BezierPath.h"
#include "SomePathLayout.h"
#include "Vector3.h"
#include "VectorList.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/14/24
 */
namespace PJ {
    /// Distribute objects along a bezier path
    class BezierPathLayout : public SomePathLayout {
    protected:
        VectorList<Vector3> controlPoints;

    public:
        BezierPathLayout() {}

        BezierPathLayout(VectorList<Vector3> const& controlPoints) :
            controlPoints(controlPoints) {}

        BezierPathLayout& SetControlPoints(VectorList<Vector3> const& value) {
            controlPoints = value;

            SetLayoutNeedsBuild();

            return *this;
        }

        BezierPathLayout& SetControlPoint(int i, Vector3 value) {
            GUARDR(i >= 0 && i < controlPoints.size(), *this)
            controlPoints[i] = value;

            SetLayoutNeedsBuild();

            return *this;
        }

        // MARK: SomePathLayout

        SP<SomePath> BuildPath() override {
            return SCAST<SomePath>(MAKE<BezierPath>(controlPoints));
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "BezierPathLayout";
        }
    };
} // namespace PJ
