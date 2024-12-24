#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple types
 CODE REVIEW: 12/18/24
 */
namespace PJ {
    /// Returns anchor position for axis length
    using LayoutAnchorFunc = std::function<float(float axisLength)>;

    /// Anchor position for views
    struct LayoutAnchor2D {
    public:
        /// Normalized x value (0-1)
        float x{};

        /// Normalized y value (0-1)
        float y{};

        Vector2 AnchorPosition(Vector2 size) const {
            return { size.x * x, size.y * y };
        }

        static const LayoutAnchor2D topLeft;
        static const LayoutAnchor2D top;
        static const LayoutAnchor2D topRight;
        static const LayoutAnchor2D left;
        static const LayoutAnchor2D center;
        static const LayoutAnchor2D right;
        static const LayoutAnchor2D bottomLeft;
        static const LayoutAnchor2D bottom;
        static const LayoutAnchor2D bottomRight;
    };
} // namespace PJ
