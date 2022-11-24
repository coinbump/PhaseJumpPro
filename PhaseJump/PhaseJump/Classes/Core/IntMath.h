#ifndef PJINTTMATH_H
#define PJINTTMATH_H

#include <math.h>

namespace PJ {
    struct IntMath {
        typedef int Type;

        static int Sqrt(int value) {
            return sqrtf((float)value);
        }
    };
}

#endif
