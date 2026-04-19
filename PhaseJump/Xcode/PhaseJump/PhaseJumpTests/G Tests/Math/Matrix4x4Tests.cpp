#include "gtest/gtest.h"

#include "Matrix4x4.h"
#include "StringUtils.h"
#include "MathUtils.h"
#include <TSMatrix4D.h>

using namespace std;
using namespace PJ;

// FUTURE: these unit tests are incomplete, need more

TEST(Matrix4x4, TestMultiplyPoint) {
    Matrix4x4 sut;
    sut.LoadScale(Vector3(2, 3, 4));
    Vector3 point(1, 2, 3);

    auto scaledPoint = sut.MultiplyPoint(point);
    EXPECT_EQ(Vector3(2, 6, 12), scaledPoint);

    sut.LoadTranslate(Vector3(1, -1, 10));
    auto translatedPoint = sut.MultiplyPoint(point);
    EXPECT_EQ(Vector3(2, 1, 13), translatedPoint);

    sut.LoadZRadRotation(M_PI);
    auto rotatedPoint = sut.MultiplyPoint(point);
    auto roundRotated = Vector3(round(rotatedPoint.x), round(rotatedPoint.y), round(rotatedPoint.z));
    EXPECT_EQ(Vector3(-1, -2, 3), roundRotated);
}

namespace {
    /// Multiplies the column-major matrix by the homogeneous column vector (x, y, z, w)
    /// and returns the resulting (cx, cy, cz, cw). Required because Matrix4x4::MultiplyPoint
    /// goes through Terathon::Transform3D, which is affine and discards the projection's
    /// w-row.
    struct Vec4 {
        float x{}, y{}, z{}, w{};
    };

    Vec4 Multiply(Matrix4x4 const& m, Vec4 v) {
        return { m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
                 m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
                 m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
                 m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w };
    }
} // namespace

TEST(Matrix4x4, LoadPerspective_Elements) {
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 2.0f, 1.0f, 10.0f);

    // f = 1 / tan(pi/4) = 1
    EXPECT_NEAR(0.5f, sut[0], 1e-6f);
    EXPECT_NEAR(1.0f, sut[5], 1e-6f);
    EXPECT_NEAR(11.0f / -9.0f, sut[10], 1e-6f);
    EXPECT_NEAR(-1.0f, sut[11], 1e-6f);
    EXPECT_NEAR(20.0f / -9.0f, sut[14], 1e-6f);
    EXPECT_NEAR(0.0f, sut[15], 1e-6f);

    // Off-diagonal entries should be zero
    EXPECT_NEAR(0.0f, sut[1], 1e-6f);
    EXPECT_NEAR(0.0f, sut[4], 1e-6f);
    EXPECT_NEAR(0.0f, sut[12], 1e-6f);
    EXPECT_NEAR(0.0f, sut[13], 1e-6f);
}

TEST(Matrix4x4, LoadPerspective_NearPlaneMapsToNdcMinusOne) {
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 1.0f, 1.0f, 10.0f);

    auto clip = Multiply(sut, { 0, 0, -1, 1 });
    EXPECT_NEAR(1.0f, clip.w, 1e-5f);
    EXPECT_NEAR(-1.0f, clip.z / clip.w, 1e-5f);
}

TEST(Matrix4x4, LoadPerspective_FarPlaneMapsToNdcPlusOne) {
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 1.0f, 1.0f, 10.0f);

    auto clip = Multiply(sut, { 0, 0, -10, 1 });
    EXPECT_NEAR(10.0f, clip.w, 1e-5f);
    EXPECT_NEAR(1.0f, clip.z / clip.w, 1e-5f);
}

TEST(Matrix4x4, LoadPerspective_RightEdgeAtNearPlaneMapsToNdcPlusOne) {
    // 90° vertical fov, aspect 1 → at z = -1 the visible half-width is 1.
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 1.0f, 1.0f, 10.0f);

    auto clip = Multiply(sut, { 1, 0, -1, 1 });
    EXPECT_NEAR(1.0f, clip.x / clip.w, 1e-5f);
    EXPECT_NEAR(0.0f, clip.y / clip.w, 1e-5f);
}

TEST(Matrix4x4, LoadPerspective_GuardZeroAspectLeavesIdentity) {
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 0.0f, 1.0f, 10.0f);

    Matrix4x4 expected;
    expected.LoadIdentity();
    EXPECT_EQ(expected, sut);
}

TEST(Matrix4x4, LoadPerspective_GuardEqualNearFarLeavesIdentity) {
    Matrix4x4 sut;
    sut.LoadPerspective((float)M_PI_2, 1.0f, 5.0f, 5.0f);

    Matrix4x4 expected;
    expected.LoadIdentity();
    EXPECT_EQ(expected, sut);
}

TEST(Matrix4x4, LoadPerspective_GuardZeroFovLeavesIdentity) {
    Matrix4x4 sut;
    sut.LoadPerspective(0.0f, 1.0f, 1.0f, 10.0f);

    Matrix4x4 expected;
    expected.LoadIdentity();
    EXPECT_EQ(expected, sut);
}

TEST(Matrix4x4, TestTerathonConversion) {
    Matrix4x4 sut;
    for (int i = 0; i < 16; i++) {
        sut[i] = i;
    }

    cout << "Matrix: A\n";
    cout << sut.ToString();

    auto tsMatrix = (Terathon::Matrix4D)sut;

    Matrix4x4 sut2(tsMatrix);

    cout << "Matrix: B\n";
    cout << sut2.ToString();

    EXPECT_EQ(sut, sut2);
}
