#ifndef PJMATRIX4x4_h
#define PJMATRIX4x4_h

#include "Vector3.h"
#include "_String.h"
#include <array>

namespace Terathon {
    class Matrix4D;
    class Transform4D;
}

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
        Matrix4x4(Terathon::Matrix4D const tsMatrix);

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

        operator Terathon::Matrix4D() const;
        operator Terathon::Transform4D() const;

        // a e i m
        // b f j n
        // c g k o
        // d h l p
        float a() const { return (*this)[0]; }
        float e() const { return (*this)[1]; }
        float i() const { return (*this)[2]; }
        float m() const { return (*this)[3]; }

        float b() const { return (*this)[4]; }
        float f() const { return (*this)[5]; }
        float j() const { return (*this)[6]; }
        float n() const { return (*this)[7]; }

        float c() const { return (*this)[8]; }
        float g() const { return (*this)[9]; }
        float k() const { return (*this)[10]; }
        float o() const { return (*this)[11]; }
        
        float d() const { return (*this)[12]; }
        float h() const { return (*this)[13]; }
        float l() const { return (*this)[14]; }
        float p() const { return (*this)[15]; }

        float& a() { return (*this)[0]; }
        float& e() { return (*this)[1]; }
        float& i() { return (*this)[2]; }
        float& m() { return (*this)[3]; }

        float& b() { return (*this)[4]; }
        float& f() { return (*this)[5]; }
        float& j() { return (*this)[6]; }
        float& n() { return (*this)[7]; }

        float& c() { return (*this)[8]; }
        float& g() { return (*this)[9]; }
        float& k() { return (*this)[10]; }
        float& o() { return (*this)[11]; }

        float& d() { return (*this)[12]; }
        float& h() { return (*this)[13]; }
        float& l() { return (*this)[14]; }
        float& p() { return (*this)[15]; }

        Matrix4x4 operator*(Matrix4x4 const& rhs) const;

        Vector3 MultiplyPoint(Vector3 point);

        String ToString() const;
    };
}

#endif
