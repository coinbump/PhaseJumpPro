#pragma once

#include "MathUtils.h"
#include <type_traits>

/*
 RATING: 5 stars
 Simple abstraction
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    /// Defines type and operations for math operations
    /// Used in template classes
    struct FloatMath {
        using Type = float;

        static constexpr Type DegreesToRadians = (Type)M_PI / (Type)180.0f;
        static constexpr Type RadiansToDegrees = 180.0f / (Type)M_PI;

        // TODO: use std::requires
        template <
            typename Floating,
            std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
        static float Sqrt(Floating value) {
            return sqrt(value);
        }

        static constexpr float maxValue = std::numeric_limits<float>::infinity();
    };

    // FUTURE: support DoubleMath if needed

    // Convenience names
    using MathF = FloatMath;
    using Mathf = FloatMath;
} // namespace PJ
