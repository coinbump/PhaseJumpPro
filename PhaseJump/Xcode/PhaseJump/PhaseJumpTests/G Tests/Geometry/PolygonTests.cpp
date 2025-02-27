#include "gtest/gtest.h"
#include "Polygon.h"

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
