#pragma once

#include "Dev.h"
#include "FloatMath.h"
#include "IntMath.h"
#include "Macros_Vectors.h"
#include "StringConvertible.h"
#include "StringUtils.h"
#include "Vector2.h"

namespace Terathon {
    class Vector3D;
    class Point3D;
} // namespace Terathon

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/6/24
 */
namespace PJ {
    class Vector3 {
    public:
        using This = Vector3;
        using MathType = float;

        float x = 0;
        float y = 0;
        float z = 0;

        constexpr Vector3() {}

        constexpr Vector3(float x, float y, float z) :
            x(x),
            y(y),
            z(z) {}

        constexpr Vector3(Vector2 value) :
            x(value.x),
            y(value.y),
            z(0) {}

        static constexpr Vector3 Uniform(float value) {
            return Vector3(value, value, value);
        }

        // TODO: cleanup using constexpr (see Vector2)
        static Vector3 const one;
        static Vector3 const zero;
        static Vector3 const forward;
        static Vector3 const back;

        constexpr operator Vector2() const {
            return Vector2(x, y);
        }

        MathType& operator[](size_t index) {
            Assert(index >= 0 && index < 3);

            /// Don't use &x[index] because struct packing isn't guaranteed
            MathType* indices[3] = { &x, &y, &z };
            return *indices[index];
        }

        MathType operator[](size_t index) const {
            Assert(index >= 0 && index < 3);

            /// Don't use &x[index] because struct packing isn't guaranteed
            MathType const* indices[3] = { &x, &y, &z };
            return *indices[index];
        }

        constexpr bool operator==(Vector3 const& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        operator Terathon::Vector3D() const;
        operator Terathon::Point3D() const;

        void Normalize() {
            float mag = Magnitude(true);
            for (int i = 0; i < 3; i++) {
                (*this)[i] = (*this)[i] / mag;
            }
        }

        MathType& AxisValue(Axis axis) {
            switch (axis) {
            case Axis::X:
                return x;
            case Axis::Y:
                return y;
            case Axis::Z:
                return z;
            }
        }

        MathType AxisValue(Axis axis) const {
            switch (axis) {
            case Axis::X:
                return x;
            case Axis::Y:
                return y;
            case Axis::Z:
                return z;
            }
        }

        VECTOR_METHODS(Vector3, MathType, 3);

        friend std::ostream& operator<<(std::ostream&, Vector3 const& value);

        // MARK: StringConvertible

        String ToString() const {
            std::stringstream stream;
            stream << "{" << x << ", " << y << ", " << z << "}";
            return stream.str();
        }
    };

    class Vector3Int {
    public:
        using This = Vector3Int;
        using MathType = int;

        int x = 0;
        int y = 0;
        int z = 0;

        Vector3Int() {}

        Vector3Int(int x, int y, int z) :
            x(x),
            y(y),
            z(z) {}

        MathType& operator[](size_t index) {
            Assert(index >= 0 && index < 3);

            /// Don't use &x[index] because struct packing isn't guaranteed
            MathType* indices[3] = { &x, &y, &z };
            return *indices[index];
        }

        MathType operator[](size_t index) const {
            Assert(index >= 0 && index < 3);

            /// Don't use &x[index] because struct packing isn't guaranteed
            MathType const* indices[3] = { &x, &y, &z };
            return *indices[index];
        }

        constexpr bool operator==(Vector3Int const& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        constexpr operator Vector2() const {
            return Vector2((float)x, (float)y);
        }

        VECTOR_METHODS(Vector3Int, int, 3);
    };

    // Convenience names
    using Vec3 = Vector3;
    using Vec3I = Vector3Int;

    // MARK: Stream friend operators

    std::ostream& operator<<(std::ostream& os, PJ::Vector3 const& value);

} // namespace PJ
