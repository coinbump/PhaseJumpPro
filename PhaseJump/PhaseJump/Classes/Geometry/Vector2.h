//
//  Vectro2.h
//  PhaseJump
//
//  Created by Jeremy Vineyard on 11/11/22.
//

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
        typedef SomeVector<FloatMath, 2> Base;

        Vector2() : Base() {}
        Vector2(float x, float y) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
        }
    };

    class Vector2Int : public SomeVector<IntMath, 2> {
    public:
        typedef SomeVector<IntMath, 2> Base;

        Vector2Int() : Base() {}
        Vector2Int(int x, int y) : Base() {
            (*this)[0] = x;
            (*this)[1] = y;
        }
    };

    namespace _Vector2 {
        static Vector2 const down = Vector2(0, -1);
        static Vector2 const left = Vector2(-1, 0);
        static Vector2 const one = Vector2(1, 1);
        static Vector2 const right = Vector2(1, 0);
        static Vector2 const up = Vector2(0, 1);
        static Vector2 const zero = Vector2(0, 0);
    }
}

#endif
