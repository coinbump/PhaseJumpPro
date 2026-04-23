#include "gtest/gtest.h"

#include "Colliders2D.h"

using namespace std;
using namespace PJ;

namespace Collider2DTests {
};

TEST(CircleCollider2D, TestHit) {
    CircleCollider2D sut(10);

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(10, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(7.071, 7.071)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 10)));
    EXPECT_FALSE(sut.TestHit(Vector2(10, 10)));
}

TEST(CircleCollider2D, TestHit_NegativeCoordinates) {
    CircleCollider2D sut(5);

    EXPECT_TRUE(sut.TestHit(Vector2(-5, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, -5)));
    EXPECT_TRUE(sut.TestHit(Vector2(-3, -4)));
    EXPECT_FALSE(sut.TestHit(Vector2(-5, -5)));
}

TEST(CircleCollider2D, TestHit_Boundary) {
    CircleCollider2D sut(1);

    EXPECT_TRUE(sut.TestHit(Vector2(1, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 1)));
    EXPECT_FALSE(sut.TestHit(Vector2(1, 0.01f)));
}

TEST(CircleCollider2D, WorldSize) {
    CircleCollider2D sut(15);

    auto size = sut.WorldSize();
    EXPECT_EQ(30, size.x);
    EXPECT_EQ(30, size.y);
    EXPECT_EQ(0, size.z);
}

TEST(CircleCollider2D, SetWorldSize) {
    CircleCollider2D sut(1);

    sut.SetWorldSize({ 20, 20, 0 });
    EXPECT_EQ(10, sut.Radius());

    auto size = sut.WorldSize();
    EXPECT_EQ(20, size.x);
    EXPECT_EQ(20, size.y);
}

TEST(CircleCollider2D, SetWorldSize_NonSquare) {
    CircleCollider2D sut(1);

    sut.SetWorldSize({ 10, 30, 0 });
    EXPECT_EQ(5, sut.Radius());
}

TEST(CircleCollider2D, GetBounds) {
    CircleCollider2D sut(5);
    auto bounds = sut.GetBounds();
    EXPECT_EQ(Vector2(0, 0), bounds.center);
    EXPECT_EQ(Vector2(5, 5), bounds.extents);
}

TEST(CircleCollider2D, TestIntersect_Overlap) {
    CircleCollider2D sut(5);

    // Bounding-box test — corner-overlaps count even if the point lies outside the circle.
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(0, 0), Vector2(1, 1))));
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(4, 4), Vector2(2, 2))));
    // Touching edge
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(6, 0), Vector2(1, 1))));
}

TEST(CircleCollider2D, TestIntersect_NoOverlap) {
    CircleCollider2D sut(5);

    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(20, 0), Vector2(1, 1))));
    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(0, -20), Vector2(1, 1))));
}
