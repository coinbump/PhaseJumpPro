#pragma once

#include <functional>

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 9/21/24
 */
namespace PJ {
    /// Transforms a normalized progress value (0-1.0) to a new normalized progress value (0-1.0)
    using EaseFunc = std::function<float(float)>;

    namespace EaseFuncs {
        extern const EaseFunc linear;
        extern const EaseFunc inSquared;
        extern const EaseFunc inCubed;
        extern const EaseFunc outSquared;
        extern const EaseFunc outCubed;
        extern const EaseFunc outElastic;
        extern const EaseFunc inBack;
        extern const EaseFunc outBack;
        extern const EaseFunc inOutBack;
    } // namespace EaseFuncs
} // namespace PJ
