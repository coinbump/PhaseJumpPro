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
                break;
            case Axis2D::Y:
                return height;
                break;
            }
        }

        std::optional<float> AxisValue(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return width;
                break;
            case Axis2D::Y:
                return height;
                break;
            }
        }

        std::optional<float>& AxisValueOrthogonal(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return height;
                break;
            case Axis2D::Y:
                return width;
                break;
            }
        }

        std::optional<float> AxisValueOrthogonal(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return height;
                break;
            case Axis2D::Y:
                return width;
                break;
            }
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

        bool IsWidthUnbounded() const {
            return width == FloatMath::maxValue;
        }

        bool IsHeightUnbounded() const {
            return height == FloatMath::maxValue;
        }

        Vector2 ToVector2(float defaultValue = 0) const {
            return { width ? *width : defaultValue, height ? *height : defaultValue };
        }
    };
} // namespace PJ
