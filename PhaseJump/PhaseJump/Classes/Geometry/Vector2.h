#ifndef PJVECTOR2_H
#define PJVECTOR2_H

#include "SomeVector.h"
#include "FloatMath.h"
#include "IntMath.h"

/*
 RATING: 5 stars
 Utility class with unit tests
 CODE REVIEW: 11/11/22
 */
namespace PJ {
    class Vector2 : public SomeVector<FloatMath, 2> {
    public:
        using Base = SomeVector<FloatMath, 2>;

        Vector2() : Base() {}
        Vector2(float x, float y) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
        }
        Vector2(Base const& base) {
            (*this) = base;
        }

        float x() const { return (*this)[0]; }
        float y() const { return (*this)[1]; }
        float& x() { return (*this)[0]; }
        float& y() { return (*this)[1]; }

        static Vector2 const down;
        static Vector2 const left;
        static Vector2 const one;
        static Vector2 const right;
        static Vector2 const up;
        static Vector2 const zero;
    };

    class Vector2Int : public SomeVector<IntMath, 2> {
    public:
        using Base = SomeVector<IntMath, 2>;

        Vector2Int() : Base() {}
        Vector2Int(int x, int y) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
        }

        int x() const { return (*this)[0]; }
        int y() const { return (*this)[1]; }
        int& x() { return (*this)[0]; }
        int& y() { return (*this)[1]; }
    };

    // Convenience names
    using Vec2 = Vector2;
}

#endif
