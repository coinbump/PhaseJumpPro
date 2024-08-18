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
            Add(Vector3(0, 1, 0));
            Add(Vector3(1, 1, 0));
            Add(Vector3(1, 0, 0));
            Add(Vector3(0, 0, 0));
        }
    };
}

using namespace PolygonTests;

TEST(Polygon, MinMax)
{
    TestPolygon sut;
    EXPECT_EQ(Vector3(0, 0, 0), sut.Min());
    EXPECT_EQ(Vector3(1, 1, 0), sut.Max());
}

TEST(Polygon, SizeCenter)
{
    TestPolygon sut;
    EXPECT_EQ(Vector3(1, 1, 0), sut.Size());
    EXPECT_EQ(Vector3(0.5f, 0.5f, 0), sut.Center());
}

TEST(Polygon, At)
{
    TestPolygon sut;
    EXPECT_EQ(Vector3(0, 1, 0), sut.At(0));
    EXPECT_EQ(Vector3(0, 1, 0), sut.At(4));
    EXPECT_EQ(Vector3(0, 0, 0), sut.At(-1));
}

TEST(Polygon, AtWithEmptyPolygon)
{
    Polygon sut;
    EXPECT_EQ(Vector3::zero, sut.At(0));
    EXPECT_EQ(Vector3::zero, sut.At(4));
    EXPECT_EQ(Vector3::zero, sut.At(-1));
}
