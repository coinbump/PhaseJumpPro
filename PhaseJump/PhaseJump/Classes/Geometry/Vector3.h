#ifndef PJVECTOR3_H
#define PJVECTOR3_H

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
    class Vector3 {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        
        Vector3() {
        }

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {
        }

        static Vector3 const one;
        static Vector3 const zero;
        static Vector3 const forward;
        static Vector3 const back;

        operator Vector2() const {
            return Vector2(x, y);
        }

        float& operator [](size_t index) {
            return (&x)[index >= 0 && index < 3 ? index : index % 3];
        }

        float const& operator [](size_t index) const {
            return (&x)[index >= 0 && index < 3 ? index : index % 3];
        }

        bool operator==(Vector3 const& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        VECTOR_METHODS(Vector3, float, 3);
    };

    class Vector3Int {
    public:
        int x = 0;
        int y = 0;
        int z = 0;

        Vector3Int() {}
        Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {
        }

        int& operator [](size_t index) {
            return (&x)[index >= 0 && index < 3 ? index : index % 3];
        }

        int const& operator [](size_t index) const {
            return (&x)[index >= 0 && index < 3 ? index : index % 3];
        }

        bool operator==(Vector3Int const& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        VECTOR_METHODS(Vector3Int, int, 3);
    };

    // Convenience names
    using Vec3 = Vector3;
}

#endif
