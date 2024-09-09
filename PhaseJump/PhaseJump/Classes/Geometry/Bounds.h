#pragma once

#include "Vector3.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/1/24
 */
namespace PJ {
    /// Bounds in 3D space
    struct Bounds {
        Vector3 center;

        // Half the size of bounds
        Vector3 extents;

        constexpr Bounds() {}

        constexpr Bounds(Vector3 center, Vector3 extents) :
            center(center),
            extents(extents) {}

        Vector3 Min() const {
            return center - extents;
        }

        Vector3 Max() const {
            return center + extents;
        }

        Vector3 Size() const {
            return extents * 2.0f;
        }

        bool TestHit(Vector3 value) const {
            auto max = Max();
            auto min = Min();

            bool result = value.x >= min.x && value.x <= max.x && value.y >= min.y &&
                          value.y <= max.y && value.z >= min.z && value.z <= max.z;

            return result;
        }

        bool TestCollide(Bounds value) const {
            auto aMin = this->Min();
            auto aMax = this->Max();
            auto bMin = value.Min();
            auto bMax = value.Max();

            bool result = aMin.x <= bMax.x && aMax.x >= bMin.x && aMin.y <= bMax.y &&
                          aMax.y >= bMin.y && aMin.z <= bMax.z && aMax.z >= bMin.z;

            return result;
        }
    };
} // namespace PJ
