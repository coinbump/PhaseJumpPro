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
 CODE REVIEW: 11/29/24
 */
namespace PJ {
    /// 2-component vector with float values
    struct Vector2 {
    public:
        using This = Vector2;

        float x{};
        float y{};

        constexpr Vector2() {}

        constexpr Vector2(float x, float y) :
            x(x),
            y(y) {}

        float& operator[](size_t index) {
            Assert(index >= 0 && index < 2);

            // Don't use &x[index] because struct packing isn't guaranteed
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

        constexpr float& AxisValueOrthogonal(Axis2D axis) {
            switch (axis) {
            case Axis2D::X:
                return y;
            case Axis2D::Y:
                return x;
            }
        }

        constexpr float AxisValueOrthogonal(Axis2D axis) const {
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
            return { value, value };
        }

        This operator-() const {
            return { -x, -y };
        }

        friend std::ostream& operator<<(std::ostream&, This const& value);

        // MARK: StringConvertible

        String ToString() const;

        static Vector2 const left;
        static Vector2 const right;
        static Vector2 const up;
        static Vector2 const down;
        static Vector2 const upLeft;
        static Vector2 const downRight;
        static Vector2 const one;
        static Vector2 const max;
    };

    /// 2-component vector with int values
    class Vector2Int {
    public:
        using This = Vector2Int;
        using MathType = int;

        MathType x{};
        MathType y{};

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

        static constexpr This Uniform(MathType value) {
            return { value, value };
        }

        This operator-() const {
            return { -x, -y };
        }

        friend std::ostream& operator<<(std::ostream&, This const& value);

        // MARK: StringConvertible

        String ToString() const;
    };

    // Convenience names
    using Vec2 = Vector2;
    using Vec2I = Vector2Int;

    // FUTURE: support alternate coordinate systems if needed
    constexpr float vecLeft = -1;
    constexpr float vecRight = 1;
    constexpr float vecUp = 1;
    constexpr float vecDown = -1;

    // MARK: Stream friend operators

    std::ostream& operator<<(std::ostream& os, PJ::Vector2 const& value);
    std::ostream& operator<<(std::ostream& os, PJ::Vector2Int const& value);

} // namespace PJ
