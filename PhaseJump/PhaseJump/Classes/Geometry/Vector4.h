#ifndef PJVECTOR4_H
#define PJVECTOR4_H

#include "FloatMath.h"
#include "IntMath.h"
#include "Vector3.h"
#include "Macros_Vectors.h"

/*
 RATING: 5 stars
 Utility class
 CODE REVIEW: 4/27/23
 */
namespace PJ {
    class Vector4 {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
        
        Vector4() {
        }

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        }

        operator Vector3() const {
            return Vector3(x, y, z);
        }

        float& operator [](size_t index) {
            return (&x)[index >= 0 && index < 4 ? index : index % 4];
        }

        float const& operator [](size_t index) const {
            return (&x)[index >= 0 && index < 4 ? index : index % 4];
        }

        bool operator==(Vector4 const& rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        VECTOR_METHODS(Vector4, float, 4);
    };

    // Convenience names
    using Vec4 = Vector4;
}

#endif
