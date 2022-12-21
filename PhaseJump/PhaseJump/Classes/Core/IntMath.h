#ifndef PJINTTMATH_H
#define PJINTTMATH_H

#include <math.h>

namespace PJ {
    struct IntMath {
        using Type = int;

        static int Sqrt(int value) {
            return sqrtf((float)value);
        }
    };

    // Convenience names
    using MathInt = IntMath;
}

#endif
