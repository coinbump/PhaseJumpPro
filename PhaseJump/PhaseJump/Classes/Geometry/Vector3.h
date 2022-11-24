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
        typedef SomeVector<FloatMath, 3> Base;

        Vector3() : Base() {}
        Vector3(float x, float y, float z) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
    };

    class Vector3Int : public SomeVector<IntMath, 3> {
    public:
        typedef SomeVector<IntMath, 3> Base;

        Vector3Int() : Base() {}
        Vector3Int(int x, int y, int z) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }
    };

    namespace _Vector3 {
        static Vector3 const one(1, 1, 1);
        static Vector3 const zero(0, 0, 0);
    }
}

#endif
