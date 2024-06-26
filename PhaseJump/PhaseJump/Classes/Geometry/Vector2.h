#ifndef PJVECTOR2_H
#define PJVECTOR2_H

#include "FloatMath.h"
#include "IntMath.h"
#include "Macros_Vectors.h"
#include "_String.h"

namespace Terathon {
    class Vector2D;
    class Point2D;
}

/*
 RATING: 5 stars
 Utility class with unit tests
 CODE REVIEW: 11/11/22
 */
namespace PJ {
    struct Vector2 {
    public:
        using This = Vector2;

        float x = 0;
        float y = 0;

        Vector2() {}
        Vector2(float x, float y) : x(x), y(y) {
        }

        static Vector2 const down;
        static Vector2 const left;
        static Vector2 const one;
        static Vector2 const right;
        static Vector2 const up;
        static Vector2 const zero;

        float& operator [](size_t index) {
            return (&x)[index >= 0 && index < 2 ? index : index % 2];
        }

        float const& operator [](size_t index) const {
            return (&x)[index >= 0 && index < 2 ? index : index % 2];
        }

        bool operator==(Vector2 const& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        operator Terathon::Vector2D() const;
        operator Terathon::Point2D() const;

        void Normalize()
        {
            float mag = Magnitude(true);
            for (int i = 0; i < 2; i++) {
                (*this)[i] = (*this)[i] / mag;
            }
        }

        VECTOR_METHODS(Vector2, float, 2);

        String ToString() const {
            std::stringstream stream;
            stream << "{" << x << ", " << y << "}";
            return stream.str();
        }

        static This Uniform(float value) { return Vector2(value, value); }
    };

    VECTOR_EXTERNALMETHODS(Vector2, float);

    class Vector2Int {
    public:
        using This = Vector2Int;

        int x = 0;
        int y = 0;

        Vector2Int() {}
        Vector2Int(int x, int y) : x(x), y(y) {
        }

        static Vector2Int const one;
        static Vector2Int const three;

        int& operator [](size_t index) {
            return (&x)[index >= 0 && index < 2 ? index : index % 2];
        }

        int const& operator [](size_t index) const {
            return (&x)[index >= 0 && index < 2 ? index : index % 2];
        }

        bool operator==(Vector2Int const& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        VECTOR_METHODS(Vector2Int, int, 2);
            
        String ToString() const {
            std::stringstream stream;
            stream << "{" << x << ", " << y << "}";
            return stream.str();
        }

        static This Uniform(int value) { return Vector2Int(value, value); }
    };

    VECTOR_EXTERNALMETHODS(Vector2Int, int);

    // Convenience names
    using Vec2 = Vector2;
    using Vec2I = Vector2Int;
}

#endif
