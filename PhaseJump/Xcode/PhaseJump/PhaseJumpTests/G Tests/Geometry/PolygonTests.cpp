#include "gtest/gtest.h"
#include "Polygon.h"
#include <cmath>

using namespace PJ;
using namespace std;

namespace PolygonTests {
    class TestPolygon : public PJ::Polygon
    {
    public:
        TestPolygon()
        {
            Add(Vector2(0, 1));
            Add(Vector2(1, 1));
            Add(Vector2(1, 0));
            Add(Vector2(0, 0));
        }
    };

    class TestWithRangePolygon : public PJ::Polygon
    {
    public:
        TestWithRangePolygon()
        {
            Add({Vector2(0, 1),
                Vector2(1, 1),
                Vector2(1, 0),
                Vector2(0, 0)});
        }
    };

    static float TriangleArea(Triangle const& t)
    {
        auto const& a = t.value[0];
        auto const& b = t.value[1];
        auto const& c = t.value[2];
        return 0.5f * std::abs(
            a.x * (b.y - c.y) +
            b.x * (c.y - a.y) +
            c.x * (a.y - b.y)
        );
    }

    static float TotalArea(VectorList<Triangle> const& triangles)
    {
        float sum = 0.0f;
        for (auto const& t : triangles) {
            sum += TriangleArea(t);
        }
        return sum;
    }

    static Vector2 Centroid(Triangle const& t)
    {
        auto const& a = t.value[0];
        auto const& b = t.value[1];
        auto const& c = t.value[2];
        return {
            (a.x + b.x + c.x) / 3.0f,
            (a.y + b.y + c.y) / 3.0f,
        };
    }
}

using namespace PolygonTests;

TEST(Polygon, MinMax)
{
    TestPolygon sut;
    EXPECT_EQ(Vector2(0, 0), sut.Min());
    EXPECT_EQ(Vector2(1, 1), sut.Max());
}

TEST(Polygon, MinMaxWithRange)
{
    TestWithRangePolygon sut;
    EXPECT_EQ(Vector2(0, 0), sut.Min());
    EXPECT_EQ(Vector2(1, 1), sut.Max());
}

TEST(Polygon, SizeCenter)
{
    TestPolygon sut;
    EXPECT_EQ(Vector2(1, 1), sut.Size());
    EXPECT_EQ(Vector2(0.5f, 0.5f), sut.Center());
}

TEST(Polygon, SizeCenterWithRange)
{
    TestWithRangePolygon sut;
    EXPECT_EQ(Vector2(1, 1), sut.Size());
    EXPECT_EQ(Vector2(0.5f, 0.5f), sut.Center());
}

TEST(Polygon, SetSize)
{
    TestPolygon sut;
    sut.SetSize({2, 2});
    EXPECT_TRUE(sut.IsBoundsInvalid());
    
    EXPECT_EQ(Vector2(2, 2), sut.Size());
    EXPECT_EQ(Vector2(0.5f, 0.5f), sut.Center());
}

TEST(Polygon, Offset)
{
    Polygon sut{{{1, 1}}};
    sut.Offset({2, 2});
    EXPECT_EQ(Vector2(3, 3), sut[0]);
    EXPECT_TRUE(sut.IsBoundsInvalid());
}

TEST(Polygon, ModAt)
{
    TestPolygon sut;
    EXPECT_EQ(Vector2(0, 1), sut.ModGet(0));
    EXPECT_EQ(Vector2(0, 1), sut.ModGet(4));
    EXPECT_EQ(Vector2(0, 0), sut.ModGet(-1));
}

TEST(Polygon, ModAtWithEmptyPolygon)
{
    Polygon sut;
    EXPECT_EQ(Vector2{}, sut.ModGet(0));
    EXPECT_EQ(Vector2{}, sut.ModGet(4));
    EXPECT_EQ(Vector2{}, sut.ModGet(-1));
}

TEST(Polygon, ArrayOperators)
{
    TestPolygon sut;
    EXPECT_EQ(Vector2(0, 1), sut[0]);
    EXPECT_EQ(Vector2(1, 0), sut[2]);
}

TEST(Polygon, TestHit_Inside) {
    TestPolygon sut;

    EXPECT_TRUE(sut.TestHit(Vector2(0.5f, 0.5f)));
    EXPECT_TRUE(sut.TestHit(Vector2(0.1f, 0.1f)));
    EXPECT_TRUE(sut.TestHit(Vector2(0.9f, 0.9f)));
}

TEST(Polygon, TestHit_Outside) {
    TestPolygon sut;

    EXPECT_FALSE(sut.TestHit(Vector2(-0.1f, 0.5f)));
    EXPECT_FALSE(sut.TestHit(Vector2(1.1f, 0.5f)));
    EXPECT_FALSE(sut.TestHit(Vector2(0.5f, -0.1f)));
    EXPECT_FALSE(sut.TestHit(Vector2(0.5f, 1.1f)));
    EXPECT_FALSE(sut.TestHit(Vector2(2, 2)));
}

TEST(Polygon, TestHit_Triangle) {
    Polygon sut{ { 0, 0 }, { 4, 0 }, { 2, 4 } };

    EXPECT_TRUE(sut.TestHit(Vector2(2, 1)));
    EXPECT_TRUE(sut.TestHit(Vector2(2, 2)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 4)));
    EXPECT_FALSE(sut.TestHit(Vector2(4, 4)));
    EXPECT_FALSE(sut.TestHit(Vector2(-1, 0)));
}

TEST(Polygon, TestHit_MakeRect) {
    auto sut = Polygon::MakeRect({ 10, 6 });

    EXPECT_TRUE(sut.TestHit(Vector2(0, 0)));
    EXPECT_TRUE(sut.TestHit(Vector2(4, 2)));
    EXPECT_TRUE(sut.TestHit(Vector2(-4, -2)));
    EXPECT_FALSE(sut.TestHit(Vector2(6, 0)));
    EXPECT_FALSE(sut.TestHit(Vector2(0, 4)));
}

TEST(Polygon, TestHit_Concave) {
    // L-shaped polygon (concave):
    //   (0,4)---(2,4)
    //     |       |
    //     |  (2,2)---(4,2)
    //     |             |
    //   (0,0)-------(4,0)
    Polygon sut{ { 0, 0 }, { 4, 0 }, { 4, 2 }, { 2, 2 }, { 2, 4 }, { 0, 4 } };

    EXPECT_TRUE(sut.TestHit(Vector2(3, 1)));
    EXPECT_TRUE(sut.TestHit(Vector2(1, 3)));
    EXPECT_TRUE(sut.TestHit(Vector2(1, 1)));

    EXPECT_FALSE(sut.TestHit(Vector2(2.1f, 2.1f)));
    EXPECT_FALSE(sut.TestHit(Vector2(3, 3)));

    EXPECT_FALSE(sut.TestHit(Vector2(-1, 2)));
    EXPECT_FALSE(sut.TestHit(Vector2(5, 1)));
}

TEST(Polygon, TestHit_Empty) {
    Polygon sut;
    EXPECT_FALSE(sut.TestHit(Vector2(0, 0)));
}

// MARK: - BuildTriangles

TEST(Polygon, BuildTriangles_Empty) {
    Polygon sut;
    auto triangles = sut.BuildTriangles();
    EXPECT_EQ(0u, triangles.size());
}

TEST(Polygon, BuildTriangles_TooFewVertices) {
    Polygon sut{ { 0, 0 }, { 1, 1 } };
    auto triangles = sut.BuildTriangles();
    EXPECT_EQ(0u, triangles.size());
}

TEST(Polygon, BuildTriangles_Triangle) {
    // A single triangle should round-trip unchanged.
    Polygon sut{ { 0, 0 }, { 4, 0 }, { 2, 4 } };
    auto triangles = sut.BuildTriangles();

    ASSERT_EQ(1u, triangles.size());

    // Area is 0.5 * base * height = 0.5 * 4 * 4 = 8.
    EXPECT_NEAR(8.0f, TriangleArea(triangles[0]), 1e-4f);

    EXPECT_TRUE(sut.TestHit(Centroid(triangles[0])));
}

TEST(Polygon, BuildTriangles_ConvexSquare) {
    // Unit square: convex, 4 vertices → exactly 2 Delaunay triangles
    // that tile the square with total area 1.
    TestPolygon sut;
    auto triangles = sut.BuildTriangles();

    ASSERT_EQ(2u, triangles.size());
    EXPECT_NEAR(1.0f, TotalArea(triangles), 1e-4f);

    for (auto const& t : triangles) {
        EXPECT_TRUE(sut.TestHit(Centroid(t)));
    }
}

TEST(Polygon, BuildTriangles_ConvexPentagon) {
    // Convex pentagon (counter-clockwise).
    // Shoelace area = 5.0
    // A convex n-gon's Delaunay triangulation has exactly n - 2 triangles.
    Polygon sut{
        { 0, 0 }, { 2, 0 }, { 3, 1 }, { 1, 2 }, { -1, 1 }
    };
    auto triangles = sut.BuildTriangles();

    ASSERT_EQ(3u, triangles.size());
    EXPECT_NEAR(5.0f, TotalArea(triangles), 1e-4f);

    for (auto const& t : triangles) {
        EXPECT_TRUE(sut.TestHit(Centroid(t)));
    }
}

TEST(Polygon, BuildTriangles_ConvexRectangle) {
    // 10x6 rect centered at origin → area 60.
    auto sut = Polygon::MakeRect({ 10, 6 });
    auto triangles = sut.BuildTriangles();

    ASSERT_EQ(2u, triangles.size());
    EXPECT_NEAR(60.0f, TotalArea(triangles), 1e-3f);

    for (auto const& t : triangles) {
        EXPECT_TRUE(sut.TestHit(Centroid(t)));
    }
}

TEST(Polygon, BuildTriangles_ConcaveL) {
    // L-shaped polygon (concave). Same shape as TestHit_Concave:
    //   (0,4)---(2,4)
    //     |       |
    //     |  (2,2)---(4,2)
    //     |             |
    //   (0,0)-------(4,0)
    //
    // Full bounding box is 4x4 = 16, the missing top-right 2x2 corner
    // removes 4, so the L has area 12.
    Polygon sut{ { 0, 0 }, { 4, 0 }, { 4, 2 }, { 2, 2 }, { 2, 4 }, { 0, 4 } };
    auto triangles = sut.BuildTriangles();

    ASSERT_GT(triangles.size(), 0u);

    // Total area must equal the L's area — triangles outside the polygon
    // (from the unconstrained Delaunay of the vertex set) must have been
    // filtered out.
    EXPECT_NEAR(12.0f, TotalArea(triangles), 1e-3f);

    // Every triangle's centroid must be inside the polygon, and in
    // particular must not lie in the missing top-right (2,2)-(4,4) corner.
    for (auto const& t : triangles) {
        auto c = Centroid(t);
        EXPECT_TRUE(sut.TestHit(c))
            << "Triangle centroid " << c.x << "," << c.y
            << " is outside the polygon";

        bool const inMissingCorner =
            c.x > 2.0f && c.x < 4.0f && c.y > 2.0f && c.y < 4.0f;
        EXPECT_FALSE(inMissingCorner)
            << "Triangle centroid " << c.x << "," << c.y
            << " falls in the concave cut-out region";
    }
}

TEST(Polygon, BuildTriangles_ConcaveArrow) {
    // Concave arrow with a V-shaped notch cut into the right edge.
    //
    //   (0,2)────────────(4,2)
    //     │               \
    //     │                (2,1)   ← reflex vertex
    //     │               /
    //   (0,0)────────────(4,0)
    //
    // Vertices CCW: (0,0), (4,0), (2,1), (4,2), (0,2). Only (2,1) is a
    // reflex vertex — the polygon is concave there.
    // Shoelace area = 6.
    Polygon sut{ { 0, 0 }, { 4, 0 }, { 2, 1 }, { 4, 2 }, { 0, 2 } };
    auto triangles = sut.BuildTriangles();

    ASSERT_GT(triangles.size(), 0u);
    EXPECT_NEAR(6.0f, TotalArea(triangles), 1e-3f);

    for (auto const& t : triangles) {
        EXPECT_TRUE(sut.TestHit(Centroid(t)));
    }
}
