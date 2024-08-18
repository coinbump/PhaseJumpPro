#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

/*
 RATING: 5 stars
 Simple abstraction
 CODE REVIEW: 5/13/24
 */
namespace PJ {
    /// Defines type and operations for math operations
    /// Used in template classes
    struct FloatMath {
        using Type = float;

        static constexpr Type DegreesToRadians = M_PI / 180.0;
        static constexpr Type RadiansToDegrees = 180.0 / M_PI;

        template <
            typename Floating,
            std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
        static float Sqrt(Floating value) {
            return sqrt(value);
        }
    };

    // FUTURE: support DoubleMath if needed

    // Convenience names
    using MathF = FloatMath;
    using Mathf = FloatMath;
} // namespace PJ
