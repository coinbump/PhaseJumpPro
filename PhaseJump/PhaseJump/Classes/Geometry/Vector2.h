#pragma once

#include "Axis.h"
#include "Dev.h"
#include "FloatMath.h"
#include "IntMath.h"
#include "Macros_Vectors.h"
#include "StringConvertible.h"
#include "StringUtils.h"

namespace Terathon {
    class Vector2D;
    class Point2D;
} // namespace Terathon

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    struct Vector2 {
    public:
        using This = Vector2;

        float x = 0;
        float y = 0;

        constexpr Vector2() {}

        constexpr Vector2(float x, float y) :
            x(x),
            y(y) {}

        float& operator[](size_t index) {
            Assert(index >= 0 && index < 2);

            /// Don't use &x[index] because struct packing isn't guaranteed
            float* indices[2] = { &x, &y };
            return *indices[index];
        }

        float operator[](size_t index) const {
            Assert(index >= 0 && index < 2);

            /// Don't use &x[index] because struct packing isn't guaranteed
            float const* indices[2] = { &x, &y };
            return *indices[index];
        }

        constexpr bool operator==(Vector2 const& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        operator Terathon::Vector2D() const;
        operator Terathon::Point2D() const;

        constexpr float& AxisValue(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return x;
            case Axis2D::Y:
                return y;
            }
        }

        constexpr float AxisValue(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return x;
            case Axis2D::Y:
                return y;
            }
        }

        constexpr float& AxisValueReverse(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return y;
            case Axis2D::Y:
                return x;
            }
        }

        constexpr float AxisValueReverse(Axis2D axis) const {
            switch (axis) {
            case Axis2D::X:
                return y;
            case Axis2D::Y:
                return x;
            }
        }

        void Normalize() {
            float mag = Magnitude(true);
            for (int i = 0; i < 2; i++) {
                (*this)[i] = (*this)[i] / mag;
            }
        }

        VECTOR_METHODS(Vector2, float, 2);

        static constexpr This Uniform(float value) {
            return Vector2(value, value);
        }

        // MARK: StringConvertible

        String ToString() const {
            std::stringstream stream;
            stream << "{" << x << ", " << y << "}";
            return stream.str();
        }
    };

    class Vector2Int {
    public:
        using This = Vector2Int;
        using MathType = int;

        MathType x = 0;
        MathType y = 0;

        constexpr Vector2Int() {}

        constexpr Vector2Int(MathType x, MathType y) :
            x(x),
            y(y) {}

        static Vector2Int const zero;
        static Vector2Int const one;
        static Vector2Int const three;

        MathType& operator[](size_t index) {
            Assert(index >= 0 && index < 2);

            MathType* indices[2] = { &x, &y };
            return *indices[index];
        }

        MathType operator[](size_t index) const {
            Assert(index >= 0 && index < 2);

            MathType const* indices[2] = { &x, &y };
            return *indices[index];
        }

        constexpr bool operator==(Vector2Int const& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        constexpr operator Vector2() const {
            return Vector2((float)x, (float)y);
        }

        VECTOR_METHODS(Vector2Int, MathType, 2);

        String ToString() const {
            std::stringstream stream;
            stream << "{" << x << ", " << y << "}";
            return stream.str();
        }

        static constexpr This Uniform(MathType value) {
            return Vector2Int(value, value);
        }
    };

    // Convenience names
    using Vec2 = Vector2;
    using Vec2I = Vector2Int;
    using Vec2i = Vector2Int;

    // FUTURE: support alternate coordinate systems if needed
    constexpr float vecLeft = -1;
    constexpr float vecRight = 1;
    constexpr float vecUp = 1;
    constexpr float vecDown = -1;

    constexpr Vector2 vec2Left{ vecLeft, 0 };
    constexpr Vector2 vec2Right{ vecRight, 0 };
    constexpr Vector2 vec2Up{ 0, vecUp };
    constexpr Vector2 vec2Down{ 0, vecDown };
    constexpr Vector2 vec2Zero{ 0, 0 };
    constexpr Vector2 vec2One{ 1, 1 };
} // namespace PJ
