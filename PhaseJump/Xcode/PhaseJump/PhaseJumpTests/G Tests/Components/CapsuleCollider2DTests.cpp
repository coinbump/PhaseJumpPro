#include "gtest/gtest.h"

#include "Colliders2D.h"

using namespace std;
using namespace PJ;

TEST(CapsuleCollider2D, TestHit_Origin) {
    CapsuleCollider2D sut(10, 2);

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
}

TEST(CapsuleCollider2D, TestHit_RectPortion) {
    CapsuleCollider2D sut(10, 2);
    // height=10, radius=2 -> rect portion y in [-3,3], x in [-2,2]

    EXPECT_TRUE(sut.TestHit(Vector2(2, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(-2, 3)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, -3)));
    EXPECT_FALSE(sut.TestHit(Vector2(2.01f, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(-2.01f, 2)));
}

TEST(CapsuleCollider2D, TestHit_TopCap) {
    CapsuleCollider2D sut(10, 2);
    // top cap: circle at (0, 3), radius 2; extends to y=5

    EXPECT_TRUE(sut.TestHit(Vector2(0, 5)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 4)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 5.01f)));
    EXPECT_FALSE(sut.TestHit(Vector2(2, 4)));  // dist from (0,3) = sqrt(5) > 2
}

TEST(CapsuleCollider2D, TestHit_BottomCap) {
    CapsuleCollider2D sut(10, 2);

    EXPECT_TRUE(sut.TestHit(Vector2(0, -5)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, -4)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, -5.01f)));
    EXPECT_FALSE(sut.TestHit(Vector2(2, -4)));
}

TEST(CapsuleCollider2D, TestHit_DegenerateToCircle) {
    // height == 2 * radius -> pure circle
    CapsuleCollider2D sut(10, 5);

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(5, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 5)));
    EXPECT_FALSE(sut.TestHit(Vector2(5, 5)));
}

TEST(CapsuleCollider2D, WorldSize) {
    CapsuleCollider2D sut(10, 3);

    auto size = sut.WorldSize();
    EXPECT_EQ(6, size.x);
    EXPECT_EQ(10, size.y);
    EXPECT_EQ(0, size.z);
}

TEST(CapsuleCollider2D, SetWorldSize_RoundTrip) {
    CapsuleCollider2D sut(1, 1);

    sut.SetWorldSize({ 8, 20, 0 });
    EXPECT_EQ(4, sut.Radius());
    EXPECT_EQ(20, sut.Height());

    auto size = sut.WorldSize();
    EXPECT_EQ(8, size.x);
    EXPECT_EQ(20, size.y);
}

TEST(CapsuleCollider2D, SetRadius_NegativeGuarded) {
    CapsuleCollider2D sut(10, 3);

    sut.SetRadius(-1);
    EXPECT_EQ(3, sut.Radius());
}

TEST(CapsuleCollider2D, SetHeight_NegativeGuarded) {
    CapsuleCollider2D sut(10, 3);

    sut.SetHeight(-1);
    EXPECT_EQ(10, sut.Height());
}
