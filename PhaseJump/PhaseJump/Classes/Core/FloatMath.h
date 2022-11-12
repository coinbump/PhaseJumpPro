#ifndef PJFLOATMATH_H
#define PJFLOATMATH_H

#include <math.h>

namespace PJ {
    class FloatMath {
    public:
        typedef float Type;

        static float Sqrt(float value) {
            return sqrtf(value);
        }
    };
}

#endif
