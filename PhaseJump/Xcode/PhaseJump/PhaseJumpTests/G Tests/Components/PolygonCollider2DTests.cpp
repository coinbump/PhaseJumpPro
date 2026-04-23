#include "gtest/gtest.h"

#include "Colliders2D.h"

using namespace std;
using namespace PJ;

TEST(PolygonCollider2D, TestHit) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(4, 4)));
    EXPECT_TRUE(sut.TestHit(Vector2(-4, -4)));
    EXPECT_FALSE(sut.TestHit(Vector2(6, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 6)));
}

TEST(PolygonCollider2D, TestHit_SetWorldSizeScalesUp) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    EXPECT_FALSE(sut.TestHit(Vector2(6, 0)));

    sut.SetWorldSize({ 20, 20, 0 });

    EXPECT_TRUE(sut.TestHit(Vector2(6, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(9, 9)));
    EXPECT_FALSE(sut.TestHit(Vector2(11, 0)));
}

TEST(PolygonCollider2D, TestHit_SetWorldSizeScalesDown) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    EXPECT_TRUE(sut.TestHit(Vector2(4, 4)));

    sut.SetWorldSize({ 4, 4, 0 });

    EXPECT_FALSE(sut.TestHit(Vector2(4, 4)));
    EXPECT_TRUE(sut.TestHit(Vector2(1, 1)));
}

TEST(PolygonCollider2D, TestHit_SetWorldSizeNonUniform) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    sut.SetWorldSize({ 20, 6, 0 });

    EXPECT_TRUE(sut.TestHit(Vector2(9, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 4)));
}

TEST(PolygonCollider2D, WorldSize_AfterSetPolygon) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 8, 6 }));

    auto size = sut.WorldSize();
    EXPECT_EQ(8, size.x);
    EXPECT_EQ(6, size.y);
}

TEST(PolygonCollider2D, WorldSize_AfterSetWorldSize) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));
    sut.SetWorldSize({ 20, 30, 0 });

    auto size = sut.WorldSize();
    EXPECT_EQ(20, size.x);
    EXPECT_EQ(30, size.y);
}

TEST(PolygonCollider2D, GetBounds) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 6 }));

    auto bounds = sut.GetBounds();
    EXPECT_EQ(Vector2(0, 0), bounds.center);
    EXPECT_EQ(Vector2(5, 3), bounds.extents);
}

TEST(PolygonCollider2D, GetBounds_AfterSetWorldSize) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));
    sut.SetWorldSize({ 20, 6, 0 });

    auto bounds = sut.GetBounds();
    EXPECT_EQ(Vector2(10, 3), bounds.extents);
}

TEST(PolygonCollider2D, TestIntersect_Overlap) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(0, 0), Vector2(1, 1))));
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(4, 4), Vector2(2, 2))));
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(6, 0), Vector2(1, 1))));
}

TEST(PolygonCollider2D, TestIntersect_NoOverlap) {
    PolygonCollider2D sut;
    sut.SetPolygon(Polygon::MakeRect({ 10, 10 }));

    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(20, 0), Vector2(1, 1))));
    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(0, 20), Vector2(1, 1))));
}
