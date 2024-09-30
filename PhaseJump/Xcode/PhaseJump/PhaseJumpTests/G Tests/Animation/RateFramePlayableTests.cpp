#include "gtest/gtest.h"
#include "RateFramePlayable.h"

using namespace PJ;
using namespace std;

namespace RateFramePlayableTests {
}

using namespace RateFramePlayableTests;

TEST(RateFramePlayable, Tests)
{
    RateFramePlayable sut(10, 10.0f);

    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0)));
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(1.0f)));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
    EXPECT_EQ(2, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
}

TEST(RateFramePlayable, DivideByZero)
{
    RateFramePlayable sut(0, 0);

    EXPECT_EQ(0, sut.FrameTime());
    EXPECT_EQ(0, sut.Duration());
}

TEST(RateFramePlayable, FrameRate)
{
    RateFramePlayable sut(48, 24);

    EXPECT_EQ(24, sut.FrameRate());
    EXPECT_EQ(2, sut.Duration());

    sut.SetFrameRate(12);
    EXPECT_EQ(4, sut.Duration());
    EXPECT_EQ(12, sut.FrameRate());
}
