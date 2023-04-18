#ifndef PJMATRIX4x4_h
#define PJMATRIX4x4_h

#include "Vector3.h"
#include <array>

/*
 RATING: 4 stars
 Works, but needs unit tests
 CODE REVIEW: 4/14/23
 */
namespace PJ {
    /// A column-major 4x4 matrix
    class Matrix4x4 : public std::array<float, 16>
    {
    public:
        Matrix4x4();

        void LoadIdentity();    // Loads identity matrix.
        void LoadScale(Vector3 scale);
        void LoadTranslate(Vector3 translate);
        void LoadPerspective(float fov_radians, float aspect, float zNear, float zFar);
        void LoadOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

        // Rotate matrix around a given axis
        // For 2D orthogonal rotation, rotate around the Z axis.
        void LoadXRadRotation(float radians);
        void LoadYRadRotation(float radians);
        void LoadZRadRotation(float radians);

        // FUTURE: void Load3RadRotation(float radians, Vector3 rotate);

        Matrix4x4 operator*(Matrix4x4 const& rhs) const;
    };
}

#endif
