#include "gtest/gtest.h"
#include "Vector2.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace Vector2Tests {
}

using namespace Vector2Tests;

TEST(Vector2, TestInit) {
    Vector2 sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);

    sut = Vector2(3, 5);
    EXPECT_EQ(3, sut[0]);
    EXPECT_EQ(5, sut[1]);
}

TEST(Vector2, TestOperators) {
    Vector2 sut(2, 2);
    EXPECT_EQ(Vector2(10, 10), sut * 5.0f);
    EXPECT_EQ(Vector2(1, 1), sut / 2.0f);

    EXPECT_EQ(Vector2(3, 3), sut + Vector2(1, 1));
    EXPECT_EQ(Vector2(1, 1), sut - Vector2(1, 1));
    EXPECT_EQ(Vector2(4, 4), sut * Vector2(2, 2));
    EXPECT_EQ(Vector2(1, 1), sut / Vector2(2, 2));

    sut *= 2;
    EXPECT_EQ(Vector2(4, 4), sut);

    sut /= 2;
    EXPECT_EQ(Vector2(2, 2), sut);

    sut *= Vector2(2, 2);
    EXPECT_EQ(Vector2(4, 4), sut);

    sut /= Vector2(2, 2);
    EXPECT_EQ(Vector2(2, 2), sut);

    sut += Vector2(2, 2);
    EXPECT_EQ(Vector2(4, 4), sut);

    sut -= Vector2(2, 2);
    EXPECT_EQ(Vector2(2, 2), sut);
}

TEST(Vector2, TestMagnitude) {
    Vector2 sut(0, 100);
    EXPECT_EQ(100, sut.Magnitude());

    sut = Vector2(1, 1);
    EXPECT_NEAR(1.414f, sut.Magnitude(), .001f);

    sut = Vector2(2, 2);
    EXPECT_NEAR(8.0f, sut.Magnitude(false), .001f);
}

TEST(Vector2, TestNormalize) {
    Vector2 sut(0, 100);
    sut.Normalize();
    EXPECT_EQ(Vector2(0, 1), sut);
}

TEST(Vector2, TestRightHandOperators) {
    Vector2 sut(4, 5);
    Vector2 result = 10.0f * sut;
    EXPECT_EQ(Vector2(40, 50), result);
}

TEST(Vector2, TestArrayOperator) {
    Vector2 sut(4, 5);
    EXPECT_EQ(4, sut[0]);
    EXPECT_EQ(5, sut[1]);
}

TEST(Vector2, TestPack) {
    Vector2 sut(0, 0);
    EXPECT_EQ(sizeof(float) * 2, sizeof(sut));
}

TEST(Vector2, TestEquality) {
    Vector2 sut(0, 0);
    Vector2 sut2(1, 4);
    Vector2 sut3(1, 4);

    EXPECT_NE(sut, sut2);
    EXPECT_EQ(sut2, sut3);
}

// MARK: - Vector2Int

TEST(Vector2Int, TestInit) {
    Vector2 sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);

    sut = Vector2(3, 5);
    EXPECT_EQ(3, sut[0]);
    EXPECT_EQ(5, sut[1]);
}

TEST(Vector2Int, TestArrayOperator) {
    Vector2Int sut(4, 5);
    EXPECT_EQ(4, sut[0]);
    EXPECT_EQ(5, sut[1]);
}

TEST(Vector2Int, TestPack) {
    Vector2Int sut(0, 0);
    EXPECT_EQ(sizeof(int) * 2, sizeof(sut));
}

TEST(Vector2Int, TestEquality) {
    Vector2Int sut(0, 0);
    Vector2Int sut2(1, 4);
    Vector2Int sut3(1, 4);

    EXPECT_NE(sut, sut2);
    EXPECT_EQ(sut2, sut3);
}
 
TEST(Vector2, AxisValue) {
    Vector2 sut(1, 3);

    EXPECT_EQ(1, sut.AxisValue(Axis2D::X));
    EXPECT_EQ(3, sut.AxisValue(Axis2D::Y));
    EXPECT_EQ(3, sut.AxisValueReverse(Axis2D::X));
    EXPECT_EQ(1, sut.AxisValueReverse(Axis2D::Y));
}

TEST(Vector2, AccessOutOfRange) {
    int assertCount = 0;
    asserter.assertFunc = [&](bool isTrue) {
        GUARD(!isTrue)
        assertCount++;
    };

    Vector2 sut(1, 3);

    EXPECT_EQ(0, assertCount);
    sut[-1];
    EXPECT_EQ(1, assertCount);
    sut[2];
    EXPECT_EQ(2, assertCount);
    sut[1];
    EXPECT_EQ(2, assertCount);

    asserter.assertFunc = {};
}
