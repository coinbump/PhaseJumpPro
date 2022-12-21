#ifndef PJVECTOR3_H
#define PJVECTOR3_H

#include "SomeVector.h"
#include "FloatMath.h"
#include "IntMath.h"

/*
 RATING: 5 stars
 Utility class with unit tests
 CODE REVIEW: 11/23/22
 */
namespace PJ {
    class Vector3 : public SomeVector<FloatMath, 3> {
    public:
        using Base = SomeVector<FloatMath, 3>;

        Vector3() : Base() {}
        Vector3(float x, float y, float z) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
        Vector3(Base const& base) {
            (*this) = base;
        }

        static Vector3 const one;
        static Vector3 const zero;
    };

    class Vector3Int : public SomeVector<IntMath, 3> {
    public:
        using Base = SomeVector<IntMath, 3>;

        Vector3Int() : Base() {}
        Vector3Int(int x, int y, int z) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
    };
}

#endif
