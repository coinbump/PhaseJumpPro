#include "Matrix4x4.h"
#include "Vector4.h"
#include <sstream>
#include <TSMatrix3D.h>
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

Matrix4x4::Matrix4x4() {
    LoadIdentity();
}

Matrix4x4::Matrix4x4(Terathon::Matrix4D const tsMatrix) {
    a() = tsMatrix.m00;
    b() = tsMatrix.m01;
    c() = tsMatrix.m02;
    d() = tsMatrix.m03;

    e() = tsMatrix.m10;
    f() = tsMatrix.m11;
    g() = tsMatrix.m12;
    h() = tsMatrix.m13;

    i() = tsMatrix.m20;
    j() = tsMatrix.m21;
    k() = tsMatrix.m22;
    l() = tsMatrix.m23;

    m() = tsMatrix.m30;
    n() = tsMatrix.m31;
    o() = tsMatrix.m32;
    p() = tsMatrix.m33;
}

Matrix4x4::operator Terathon::Matrix4D() const {
    auto result = Terathon::Matrix4D(
        a(), b(), c(), d(), e(), f(), g(), h(), i(), j(), k(), l(), m(), n(), o(), p()
    );
    return result;
}

Matrix4x4::operator Terathon::Transform3D() const {
    auto result = Terathon::Transform3D(a(), b(), c(), d(), e(), f(), g(), h(), i(), j(), k(), l());
    return result;
}

String Matrix4x4::ToString() const {
    stringstream s;
    s << a() << e() << i() << m() << std::endl;
    s << b() << f() << j() << n() << std::endl;
    s << c() << g() << k() << o() << std::endl;
    s << d() << h() << l() << p() << std::endl;

    return s.str();
}

// Load the identity matrix
void Matrix4x4::LoadIdentity() {
    (*this)[0] = 1.0f;
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = 1.0f;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = 1.0f;
    (*this)[11] = 0.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 0.0f;
    (*this)[15] = 1.0f;
}

void Matrix4x4::LoadScale(Vector3 scale) {
    // Tested and works
    (*this)[0] = scale[0];
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = scale[1];
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = scale[2];
    (*this)[11] = 0.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 0.0f;
    (*this)[15] = 1.0f;
}

void Matrix4x4::LoadXRadRotation(float radians) {
    float cosrad = cosf(radians);
    float sinrad = sinf(radians);

    (*this)[0] = 1.0f;
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = cosrad;
    (*this)[6] = sinrad;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = -sinrad;
    (*this)[10] = cosrad;
    (*this)[11] = 0.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 0.0f;
    (*this)[15] = 1.0f;
}

void Matrix4x4::LoadYRadRotation(float radians) {
    float cosrad = cosf(radians);
    float sinrad = sinf(radians);

    (*this)[0] = cosrad;
    (*this)[1] = 0.0f;
    (*this)[2] = -sinrad;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = 1.0f;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = sinrad;
    (*this)[9] = 0.0f;
    (*this)[10] = cosrad;
    (*this)[11] = 0.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 0.0f;
    (*this)[15] = 1.0f;
}

/*
    Matrix4x4::LoadZRadRotation

    Applies a scaling transformation to the matrix.

 */
void Matrix4x4::LoadZRadRotation(float radians) {
    float cosrad = cosf(radians);
    float sinrad = sinf(radians);

    (*this)[0] = cosrad;
    (*this)[1] = sinrad;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = -sinrad;
    (*this)[5] = cosrad;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = 1.0f;
    (*this)[11] = 0.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 0.0f;
    (*this)[15] = 1.0f;
}

/// Load a translation matrix
void Matrix4x4::LoadTranslate(Vector3 translate) {
    (*this)[0] = 1.0f;
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = 1.0f;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = 1.0f;
    (*this)[11] = 0.0f;

    (*this)[12] = translate[0];
    (*this)[13] = translate[1];
    (*this)[14] = translate[2];
    (*this)[15] = 1.0f;
}

/**
    Matrix4x4::LoadPerspective

    Parameters:
    `fov_radians`: field of view in the y direction
    `aspect`: aspect ratio in the x direction (height)
    `zNear`: distance from viewer to the near clipping plane (always positive)
    `zFar`: distance from viewer to the far clipping plane (always positive)
 */
void Matrix4x4::LoadPerspective(float fov_radians, float aspect, float zNear, float zFar) {
    float f = 1.0f / tanf(fov_radians / 2.0f);

    (*this)[0] = f / aspect;
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = f;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = (zFar + zNear) / (zNear - zFar);
    (*this)[11] = -1.0f;

    (*this)[12] = 0.0f;
    (*this)[13] = 0.0f;
    (*this)[14] = 2 * zFar * zNear / (zNear - zFar);
    (*this)[15] = 0.0f;
}

/**
    Loads an orthogonal matrix for 2D projection of a 3D space.

    WIN32: far, near are reserved in Visual Studio. Use zNear/zFar for
   portability.

    https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix.html

 */
void Matrix4x4::LoadOrthographic(
    float left, float right, float bottom, float top, float zNear, float zFar
) {
    // TODO: is this correct? far and near might be switched? (create scene to test)
    float r_l = right - left;
    float t_b = top - bottom;
    float f_n = zFar - zNear;
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(zFar + zNear) / (zFar - zNear);

    (*this)[0] = 2.0f / r_l;
    (*this)[1] = 0.0f;
    (*this)[2] = 0.0f;
    (*this)[3] = 0.0f;

    (*this)[4] = 0.0f;
    (*this)[5] = 2.0f / t_b;
    (*this)[6] = 0.0f;
    (*this)[7] = 0.0f;

    (*this)[8] = 0.0f;
    (*this)[9] = 0.0f;
    (*this)[10] = -2.0f / f_n;
    (*this)[11] = 0.0f;

    (*this)[12] = tx;
    (*this)[13] = ty;
    (*this)[14] = tz;
    (*this)[15] = 1.0f;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
    Matrix4x4 result;

    result[0] =
        (*this)[0] * rhs[0] + (*this)[4] * rhs[1] + (*this)[8] * rhs[2] + (*this)[12] * rhs[3];
    result[1] =
        (*this)[1] * rhs[0] + (*this)[5] * rhs[1] + (*this)[9] * rhs[2] + (*this)[13] * rhs[3];
    result[2] =
        (*this)[2] * rhs[0] + (*this)[6] * rhs[1] + (*this)[10] * rhs[2] + (*this)[14] * rhs[3];
    result[3] =
        (*this)[3] * rhs[0] + (*this)[7] * rhs[1] + (*this)[11] * rhs[2] + (*this)[15] * rhs[3];

    result[4] =
        (*this)[0] * rhs[4] + (*this)[4] * rhs[5] + (*this)[8] * rhs[6] + (*this)[12] * rhs[7];
    result[5] =
        (*this)[1] * rhs[4] + (*this)[5] * rhs[5] + (*this)[9] * rhs[6] + (*this)[13] * rhs[7];
    result[6] =
        (*this)[2] * rhs[4] + (*this)[6] * rhs[5] + (*this)[10] * rhs[6] + (*this)[14] * rhs[7];
    result[7] =
        (*this)[3] * rhs[4] + (*this)[7] * rhs[5] + (*this)[11] * rhs[6] + (*this)[15] * rhs[7];

    result[8] =
        (*this)[0] * rhs[8] + (*this)[4] * rhs[9] + (*this)[8] * rhs[10] + (*this)[12] * rhs[11];
    result[9] =
        (*this)[1] * rhs[8] + (*this)[5] * rhs[9] + (*this)[9] * rhs[10] + (*this)[13] * rhs[11];
    result[10] =
        (*this)[2] * rhs[8] + (*this)[6] * rhs[9] + (*this)[10] * rhs[10] + (*this)[14] * rhs[11];
    result[11] =
        (*this)[3] * rhs[8] + (*this)[7] * rhs[9] + (*this)[11] * rhs[10] + (*this)[15] * rhs[11];

    result[12] =
        (*this)[0] * rhs[12] + (*this)[4] * rhs[13] + (*this)[8] * rhs[14] + (*this)[12] * rhs[15];
    result[13] =
        (*this)[1] * rhs[12] + (*this)[5] * rhs[13] + (*this)[9] * rhs[14] + (*this)[13] * rhs[15];
    result[14] =
        (*this)[2] * rhs[12] + (*this)[6] * rhs[13] + (*this)[10] * rhs[14] + (*this)[14] * rhs[15];
    result[15] =
        (*this)[3] * rhs[12] + (*this)[7] * rhs[13] + (*this)[11] * rhs[14] + (*this)[15] * rhs[15];

    return result;
}

Vector3 Matrix4x4::MultiplyPoint(Vector3 point) const {
    Terathon::Transform3D tsTransform = *this;
    Terathon::Point3D tsPoint(point.x, point.y, point.z);

    auto result = tsTransform * tsPoint;
    return Vector3(result.x, result.y, result.z);
} // Tested

void Matrix4x4::Inverse() {
    Terathon::Transform3D tsTransform = *this;
    tsTransform = Terathon::Inverse(tsTransform);
    *this = tsTransform;
} // Tested
