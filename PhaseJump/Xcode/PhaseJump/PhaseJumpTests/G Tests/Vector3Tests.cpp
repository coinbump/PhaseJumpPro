#include "gtest/gtest.h"
#include "Vector3.h"
#include "GeoTransform.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace Vector3Tests {
}

using namespace Vector3Tests;

TEST(Vector3, TestInit) {
    Vector3 sut(1, 2, 3);
    EXPECT_EQ(1, sut[0]);
    EXPECT_EQ(2, sut[1]);
    EXPECT_EQ(3, sut[2]);
}

TEST(Vector3, TestInit_Default) {
    Vector3 sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);
    EXPECT_EQ(0, sut[2]);
}

TEST(Vector3, TestArrayOperator) {
    Vector3 sut(4, 5, 6);
    EXPECT_EQ(4, sut[0]);
    EXPECT_EQ(5, sut[1]);
    EXPECT_EQ(6, sut[2]);
}

TEST(Vector3, TestPack) {
    Vector3 sut(0, 0, 0);
    auto sutSize = sizeof(sut);
    EXPECT_EQ(sizeof(float) * 3, sutSize);
}

TEST(Vector3, TestEquality) {
    Vector2Int sut(0, 0);
    Vector2Int sut2(1, 4);
    Vector2Int sut3(1, 4);

    EXPECT_NE(sut, sut2);
    EXPECT_EQ(sut2, sut3);
}

TEST(Vector3, OperatorStream) {
    Vector3 sut(1, 2, 3);
    ostringstream ss;
    ss << sut;
    EXPECT_EQ("{1, 2, 3}", ss.str());
}

// MARK: - Vector3Int

TEST(Vector3Int, TestInit) {
    Vector3Int sut(1, 2, 3);
    EXPECT_EQ(1, sut[0]);
    EXPECT_EQ(2, sut[1]);
    EXPECT_EQ(3, sut[2]);
}

TEST(Vector3Int, TestInit_Default) {
    Vector3Int sut;
    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(0, sut[1]);
    EXPECT_EQ(0, sut[2]);
}

TEST(Vector3Int, TestArrayOperator) {
    Vector3Int sut(4, 5, 6);
    EXPECT_EQ(4, sut[0]);
    EXPECT_EQ(5, sut[1]);
    EXPECT_EQ(6, sut[2]);
}

TEST(Vector3Int, TestPack) {
    Vector3Int sut(0, 0, 0);
    auto sutSize = sizeof(sut);
    EXPECT_EQ(sizeof(int) * 3, sutSize);
}

TEST(Vector3Int, TestEquality) {
    Vector2Int sut(0, 0);
    Vector2Int sut2(1, 4);
    Vector2Int sut3(1, 4);

    EXPECT_NE(sut, sut2);
    EXPECT_EQ(sut2, sut3);
}

TEST(Vector3, AxisValue) {
    Vector3 sut(1, 3, 4);

    EXPECT_EQ(1, sut.AxisValue(Axis::X));
    EXPECT_EQ(3, sut.AxisValue(Axis::Y));
    EXPECT_EQ(4, sut.AxisValue(Axis::Z));
}

TEST(Vector3, AccessOutOfRange) {
    int assertCount = 0;
    assertFunc = [&](bool isTrue) {
        GUARD(!isTrue)
        assertCount++;
    };

    Vector3 sut(1, 3, 4);

    EXPECT_EQ(0, assertCount);
    sut[-1];
    EXPECT_EQ(1, assertCount);
    sut[3];
    EXPECT_EQ(2, assertCount);
    sut[2];
    EXPECT_EQ(2, assertCount);

    assertFunc = {};
}
