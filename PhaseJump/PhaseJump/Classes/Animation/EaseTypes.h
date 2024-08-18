#pragma once

#define _USE_MATH_DEFINES

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/13/24
 */
namespace PJ {
    /// For editors to pick from list of types
    enum class EaseType {
        Linear,

        InSquared,
        InCubed,
        OutSquared,
        OutCubed,
        OutElastic,
        InBack,
        OutBack,
        InOutBack
    };
} // namespace PJ
