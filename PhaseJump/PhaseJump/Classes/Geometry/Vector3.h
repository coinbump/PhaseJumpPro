#ifndef PJVECTOR3_H
#define PJVECTOR3_H

#include "SomeVector.h"
#include "FloatMath.h"
#include "IntMath.h"
#include "Vector2.h"
#include "Macros_Vectors.h"

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
            (*this)[0] = base[0];
            (*this)[1] = base[1];
            (*this)[2] = base[2];
        }

        static Vector3 const one;
        static Vector3 const zero;
        static Vector3 const forward;
        static Vector3 const back;

        float x() const { return (*this)[0]; }
        float y() const { return (*this)[1]; }
        float z() const { return (*this)[2]; }
        float& x() { return (*this)[0]; }
        float& y() { return (*this)[1]; }
        float& z() { return (*this)[2]; }

        operator Vector2() const {
            return Vector2(x(), y());
        }

        VECTOR_METHODS(Vector3, 3);
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

        VECTOR_METHODS(Vector3Int, 3);
    };

    // Convenience names
    using Vec3 = Vector3;
}

#endif
