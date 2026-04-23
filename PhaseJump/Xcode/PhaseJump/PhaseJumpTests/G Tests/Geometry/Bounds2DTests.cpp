#include "gtest/gtest.h"
#include "Bounds2D.h"

using namespace PJ;
using namespace std;

namespace Bounds2DTests {}

using namespace Bounds2DTests;

TEST(Bounds2D, Min_Max_Size)
{
    Bounds2D sut(Vector2(1, 2), Vector2(3, 4));
    EXPECT_EQ(Vector2(-2, -2), sut.Min());
    EXPECT_EQ(Vector2(4, 6), sut.Max());
    EXPECT_EQ(Vector2(6, 8), sut.Size());
}

TEST(Bounds2D, TestHit)
{
    Bounds2D sut(Vector2(0, 1), Vector2(1, 2));
    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(1, 3)));
    EXPECT_TRUE(sut.TestHit(Vector2(-1, -1)));
    EXPECT_FALSE(sut.TestHit(Vector2(-2, -1)));
    EXPECT_FALSE(sut.TestHit(Vector2(-1, -2)));
    EXPECT_FALSE(sut.TestHit(Vector2(2, 3)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 4)));
}

TEST(Bounds2D, TestIntersect_Overlap)
{
    Bounds2D sut(Vector2(0, 0), Vector2(2, 2));

    // Fully inside
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(0, 0), Vector2(1, 1))));
    // Partial overlap
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(1, 1), Vector2(1, 1))));
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(-1, 1), Vector2(1, 1))));
    // Touching (edges meet)
    EXPECT_TRUE(sut.TestIntersect(Bounds2D(Vector2(2, 2), Vector2(1, 1))));
}

TEST(Bounds2D, TestIntersect_NoOverlap)
{
    Bounds2D sut(Vector2(0, 0), Vector2(2, 2));

    // Completely separate on X
    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(10, 0), Vector2(1, 1))));
    // Completely separate on Y
    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(0, 10), Vector2(1, 1))));
    // Diagonal miss
    EXPECT_FALSE(sut.TestIntersect(Bounds2D(Vector2(4, 3), Vector2(1, 1))));
}

TEST(Bounds2D, DefaultConstructed_IsZero)
{
    Bounds2D sut;
    EXPECT_EQ(Vector2(0, 0), sut.center);
    EXPECT_EQ(Vector2(0, 0), sut.extents);
    EXPECT_EQ(Vector2(0, 0), sut.Size());
    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(1, 0)));
}
