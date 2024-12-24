#pragma once

#include "Rect.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Utilities for cartesian coordinate system
    namespace CartesianUtils {
        Rect CenteredIn(Rect value, Vector2 outerSize);
    } // namespace CartesianUtils
} // namespace PJ
