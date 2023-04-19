#include "gtest/gtest.h"
#include "CirclePath.h"

using namespace PJ;
using namespace std;

namespace PathTests {
    class TestCirclePath : public CirclePath
    {
    public:
        TestCirclePath() : CirclePath(1.0f)
        {
        }
    };
}

using namespace PathTests;

TEST(Polygon, TestCirclePath)
{
    TestCirclePath sut;
    EXPECT_NEAR(0, sut.PositionAt(0).x, 0.001f);
    EXPECT_NEAR(1.0f, sut.PositionAt(0).y, 0.001f);

    EXPECT_EQ(1.0f, sut.PositionAt(90.0f/360.0f).x);
    EXPECT_NEAR(0, sut.PositionAt(90.0f/360.0f).y, 0.001f);

    EXPECT_NEAR(0, sut.PositionAt(180.0f/360.0f).x, 0.001f);
    EXPECT_NEAR(-1.0f, sut.PositionAt(180.0f/360.0f).y, 0.001f);
}
