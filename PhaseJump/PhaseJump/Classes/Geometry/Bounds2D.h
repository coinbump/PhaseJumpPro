#pragma once

#include "Vector2.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/23/26
 */
namespace PJ {
    /// Axis-aligned bounds in 2D space. Mirrors Bounds
    struct Bounds2D {
        Vector2 center;

        /// Half the size of the bounds.
        Vector2 extents;

        constexpr Bounds2D() {}

        constexpr Bounds2D(Vector2 center, Vector2 extents) :
            center(center),
            extents(extents) {}

        constexpr bool operator==(Bounds2D const&) const = default;

        Vector2 Min() const {
            return center - extents;
        }

        Vector2 Max() const {
            return center + extents;
        }

        Vector2 Size() const {
            return extents * 2.0f;
        }

        bool TestHit(Vector2 value) const {
            auto max = Max();
            auto min = Min();

            return value.x >= min.x && value.x <= max.x && value.y >= min.y && value.y <= max.y;
        }

        bool TestIntersect(Bounds2D value) const {
            auto aMin = this->Min();
            auto aMax = this->Max();
            auto bMin = value.Min();
            auto bMax = value.Max();

            return aMin.x <= bMax.x && aMax.x >= bMin.x && aMin.y <= bMax.y && aMax.y >= bMin.y;
        }
    };
} // namespace PJ
