#ifndef PJFLOATMATH_H
#define PJFLOATMATH_H

#define _USE_MATH_DEFINES
#include <math.h>

namespace PJ {
    struct FloatMath {
        using Type = float;

        static constexpr float DegreesToRadians = M_PI / 180.0f;
        static constexpr float RadiansToDegrees = 180.0 / M_PI;

        static float Sqrt(float value) {
            return sqrtf(value);
        }
    };

    // Convenience names
    using MathF = FloatMath;
}

#endif
