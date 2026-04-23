#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/16/24
 */
namespace PJ {
    /// Proposed view size when creating a layout
    struct ViewSizeProposal {
        using This = ViewSizeProposal;

        std::optional<float> width;
        std::optional<float> height;

        std::optional<float>& AxisValue(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return width;
            case Axis2D::Y:
                return height;
            }
            return width;
        }

        std::optional<float> AxisValue(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return width;
            case Axis2D::Y:
                return height;
            }
            return width;
        }

        std::optional<float>& AxisValueOrthogonal(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return height;
            case Axis2D::Y:
                return width;
            }
            return height;
        }

        std::optional<float> AxisValueOrthogonal(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return height;
            case Axis2D::Y:
                return width;
            }
            return height;
        }

        /// Replaces missing optional values with the specified value
        This ReplacingNullOptionals(float value) {
            This result = *this;
            if (!result.width) {
                result.width = value;
            }
            if (!result.height) {
                result.height = value;
            }
            return result;
        }

        /// Propose the minimum size
        static const This min;

        /// Propose the ideal size
        static const This ideal;

        /// Sentinel for an unbounded axis value (width or height)
        static std::optional<float> Unbounded() {
            return FloatMath::maxValue;
        }

        bool IsWidthUnbounded() const {
            return width == Unbounded();
        }

        bool IsHeightUnbounded() const {
            return height == Unbounded();
        }

        Vector2 ToVector2(float defaultValue = 0) const {
            return { width ? *width : defaultValue, height ? *height : defaultValue };
        }
    };
} // namespace PJ
