#pragma once

#include "Dev.h"
#include "FloatMath.h"
#include "IntMath.h"
#include "Macros_Vectors.h"
#include "Vector3.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: /23
 */
// TODO: needs unit tests
// namespace PJ {
//    class Vector4 {
//    public:
//        float x = 0;
//        float y = 0;
//        float z = 0;
//        float w = 0;
//
//        Vector4() {}
//
//        Vector4(float x, float y, float z, float w) :
//            x(x),
//            y(y),
//            z(z),
//            w(w) {}
//
//        operator Vector3() const {
//            return Vector3(x, y, z);
//        }
//
//        float& operator[](size_t index) {
//        }
//
//        float const& operator[](size_t index) const {
//        }
//
//        bool operator==(Vector4 const& rhs) const {
//            return x == rhs.x && y == rhs.y && z == rhs.z;
//        }
//
//        VECTOR_METHODS(Vector4, float, 4);
//    };
//
//    // Convenience names
//    using Vec4 = Vector4;
//} // namespace PJ
