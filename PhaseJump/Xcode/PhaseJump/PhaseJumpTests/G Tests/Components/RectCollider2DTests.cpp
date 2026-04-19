#include "gtest/gtest.h"

#include "Colliders2D.h"

using namespace std;
using namespace PJ;

TEST(RectCollider2D, TestHit) {
    RectCollider2D sut({ 10, 10 });

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(4, 4)));
    EXPECT_TRUE(sut.TestHit(Vector2(-4, -4)));
    EXPECT_FALSE(sut.TestHit(Vector2(6, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 6)));
}

TEST(RectCollider2D, TestHit_Boundary) {
    RectCollider2D sut({ 10, 10 });

    EXPECT_TRUE(sut.TestHit(Vector2(5, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 5)));
    EXPECT_TRUE(sut.TestHit(Vector2(5, 5)));
    EXPECT_FALSE(sut.TestHit(Vector2(5.01f, 0)));
}

TEST(RectCollider2D, TestHit_NonSquare) {
    RectCollider2D sut({ 20, 6 });

    EXPECT_TRUE(sut.TestHit(Vector2(9, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 4)));
    EXPECT_TRUE(sut.TestHit(Vector2(-10, 3)));
}

TEST(RectCollider2D, WorldSize) {
    RectCollider2D sut({ 8, 6 });

    auto size = sut.WorldSize();
    EXPECT_EQ(8, size.x);
    EXPECT_EQ(6, size.y);
    EXPECT_EQ(0, size.z);
}

TEST(RectCollider2D, SetWorldSize_RoundTrip) {
    RectCollider2D sut;
    sut.SetWorldSize({ 20, 30, 0 });

    auto size = sut.WorldSize();
    EXPECT_EQ(20, size.x);
    EXPECT_EQ(30, size.y);
}

TEST(RectCollider2D, SetWorldSize_ResizesHitRegion) {
    RectCollider2D sut({ 10, 10 });

    EXPECT_FALSE(sut.TestHit(Vector2(6, 0)));

    sut.SetWorldSize({ 20, 20, 0 });

    EXPECT_TRUE(sut.TestHit(Vector2(6, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(9, 9)));
    EXPECT_FALSE(sut.TestHit(Vector2(11, 0)));
}

TEST(RectCollider2D, SetSize_NegativeGuarded) {
    RectCollider2D sut({ 10, 10 });

    sut.SetSize({ -1, 5 });
    auto size = sut.Size();
    EXPECT_EQ(10, size.x);
    EXPECT_EQ(10, size.y);
}
