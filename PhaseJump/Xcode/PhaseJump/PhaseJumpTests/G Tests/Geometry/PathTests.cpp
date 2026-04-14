#include "gtest/gtest.h"
#include "CirclePath.h"

using namespace PJ;
using namespace std;

namespace PathTests {
    class TestCirclePath
    {
    public:
        CirclePath path;
        
        TestCirclePath() : path(1.0f)
        {
        }
    };
}

using namespace PathTests;

TEST(CirclePath, TestCirclePath)
{
    TestCirclePath sut;
    EXPECT_NEAR(0, sut.path.PositionAt(0).x, 0.001f);
    EXPECT_NEAR(1.0f, sut.path.PositionAt(0).y, 0.001f);

    EXPECT_EQ(1.0f, sut.path.PositionAt(90.0f/360.0f).x);
    EXPECT_NEAR(0, sut.path.PositionAt(90.0f/360.0f).y, 0.001f);

    EXPECT_NEAR(0, sut.path.PositionAt(180.0f/360.0f).x, 0.001f);
    EXPECT_NEAR(-1.0f, sut.path.PositionAt(180.0f/360.0f).y, 0.001f);
}
