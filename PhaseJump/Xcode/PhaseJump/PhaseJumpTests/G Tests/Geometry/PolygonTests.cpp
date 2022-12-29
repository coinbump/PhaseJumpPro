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
            vertices.Add(Vector3(0, 1, 0));
            vertices.Add(Vector3(1, 1, 0));
            vertices.Add(Vector3(1, 0, 0));
            vertices.Add(Vector3(0, 0, 0));
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
