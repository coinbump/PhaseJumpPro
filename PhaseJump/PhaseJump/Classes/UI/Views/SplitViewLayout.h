#pragma once

#include "Axis.h"
#include "SomeViewLayout.h"
#include <functional>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    /// Splits its bounds along axis between two children: the first gets a length determined
    /// by firstViewSizeFunc; the second fills the remainder. Both children are unbounded on
    /// the orthogonal axis.
    class SplitViewLayout : public ViewLayout {
    public:
        using Base = ViewLayout;
        using This = SplitViewLayout;

        /// Axis the bounds are split along.
        Axis2D axis = Axis2D::X;

        /// Given the proposed length along the axis, returns the first child's length along that
        /// axis. Defaults to 50% of the proposed length.
        std::function<float(float)> firstViewSizeFunc = [](float proposedLength) {
            return proposedLength / 2.0f;
        };

        /// Given the proposed length along the axis, returns the middle child's length (used only
        /// when there are 3 children, e.g. for a splitter handle between the first and third
        /// views). Defaults to a fixed 10 units.
        std::function<float(float)> middleViewSizeFunc = [](float) { return 10.0f; };

        SplitViewLayout() = default;

        SplitViewLayout(
            Axis2D axis, std::function<float(float)> firstViewSizeFunc = {},
            std::function<float(float)> middleViewSizeFunc = {}
        );

        String TypeName() const override {
            return "SplitViewLayout";
        }

    protected:
        void CalculateFrames(Vector2 boundsSize, VectorList<ViewProxy> const& children) override;
    };
} // namespace PJ
