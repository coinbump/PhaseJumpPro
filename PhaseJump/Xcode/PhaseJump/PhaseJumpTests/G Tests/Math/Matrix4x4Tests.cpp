#include "gtest/gtest.h"

#include "Matrix4x4.h"
#include "_String.h"
#include <cmath>
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
