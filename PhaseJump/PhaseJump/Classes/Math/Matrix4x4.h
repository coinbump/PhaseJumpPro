#pragma once

#include "StringUtils.h"
#include "Vector3.h"
#include <array>

namespace Terathon {
    class Matrix4D;
    class Transform3D;
} // namespace Terathon

/*
 RATING: 4 stars
 Tested and works
 CODE REVIEW: 1/9/25
 */
namespace PJ {
    /// A column-major 4x4 matrix
    struct Matrix4x4 {
        static constexpr size_t size = 16;

        std::array<float, size> value;

        Matrix4x4();
        Matrix4x4(Terathon::Matrix4D const tsMatrix);

        float* data() noexcept {
            return value.data();
        }

        const float* data() const noexcept {
            return value.data();
        }

        float ModValue(size_t index) const {
            return value[index >= 0 && index < size ? index : index % size];
        }

        float& ModValue(size_t index) {
            return value[index >= 0 && index < size ? index : index % size];
        }

        float operator[](size_t index) const {
            GUARDR(index >= 0 && index < size, 0.0f)
            return value[index];
        }

        float& operator[](size_t index) {
            GUARDR(index >= 0 && index < size, value[0])
            return value[index];
        }

        bool operator==(Matrix4x4 const& rhs) const {
            return value == rhs.value;
        }

        /// Loads identity matrix
        void LoadIdentity();

        /// Loads scale matrix
        void LoadScale(Vector3 scale);

        /// Loads translation matrix
        void LoadTranslate(Vector3 translate);

        /**
            Loads a perspective transformation matrix

         @param fov_radians Field of view in the y direction
         @param aspect Aspect ratio in the x direction (height)
         @param zNear Distance from viewer to the near clipping plane (always positive)
         @param zFar Distance from viewer to the far clipping plane (always positive)
         */
        void LoadPerspective(float fov_radians, float aspect, float zNear, float zFar);

        /**
            Loads an orthogonal projection matrix

            Note: `far`, `near` are reserved in Visual Studio. Use `zNear`, `zFar` for portability.
         */
        void
        LoadOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

        /// Loads a X axis rotational matrix
        void LoadXRadRotation(float radians);

        /// Loads a Y axis rotational matrix
        void LoadYRadRotation(float radians);

        /// Loads a Z axis rotational matrix (use for 2D rotation with orthographic camera)
        void LoadZRadRotation(float radians);

        /// Inverts the matrix
        void Inverse();

        operator Terathon::Matrix4D() const;
        operator Terathon::Transform3D() const;

        // a e i m
        // b f j n
        // c g k o
        // d h l p
        float a() const {
            return (*this)[0];
        }

        float e() const {
            return (*this)[1];
        }

        float i() const {
            return (*this)[2];
        }

        float m() const {
            return (*this)[3];
        }

        float b() const {
            return (*this)[4];
        }

        float f() const {
            return (*this)[5];
        }

        float j() const {
            return (*this)[6];
        }

        float n() const {
            return (*this)[7];
        }

        float c() const {
            return (*this)[8];
        }

        float g() const {
            return (*this)[9];
        }

        float k() const {
            return (*this)[10];
        }

        float o() const {
            return (*this)[11];
        }

        float d() const {
            return (*this)[12];
        }

        float h() const {
            return (*this)[13];
        }

        float l() const {
            return (*this)[14];
        }

        float p() const {
            return (*this)[15];
        }

        float& a() {
            return (*this)[0];
        }

        float& e() {
            return (*this)[1];
        }

        float& i() {
            return (*this)[2];
        }

        float& m() {
            return (*this)[3];
        }

        float& b() {
            return (*this)[4];
        }

        float& f() {
            return (*this)[5];
        }

        float& j() {
            return (*this)[6];
        }

        float& n() {
            return (*this)[7];
        }

        float& c() {
            return (*this)[8];
        }

        float& g() {
            return (*this)[9];
        }

        float& k() {
            return (*this)[10];
        }

        float& o() {
            return (*this)[11];
        }

        float& d() {
            return (*this)[12];
        }

        float& h() {
            return (*this)[13];
        }

        float& l() {
            return (*this)[14];
        }

        float& p() {
            return (*this)[15];
        }

        Matrix4x4 operator*(Matrix4x4 const& rhs) const;

        Vector3 MultiplyPoint(Vector3 point) const;

        String ToString() const;
    };
} // namespace PJ
