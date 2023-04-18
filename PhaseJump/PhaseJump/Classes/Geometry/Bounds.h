#ifndef PJBOUNDS_H
#define PJBOUNDS_H

#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/16/23
 */
namespace PJ {
    /// Bounds in 3D space
    struct Bounds {
        Vector3 center = Vector3::zero;

        // Half the size of bounds
        Vector3 extents = Vector3::zero;

        Bounds() {
        }

        Bounds(Vector3 center, Vector3 extents) : center(center), extents(extents) {
        }

        Vector3 Min() const { return center - extents; }
        Vector3 Max() const { return center + extents; }
        Vector3 Size() const { return extents * 2.0f; }
    };
}

#endif
