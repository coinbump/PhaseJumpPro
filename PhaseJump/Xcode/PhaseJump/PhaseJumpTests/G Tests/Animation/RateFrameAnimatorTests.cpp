#include "gtest/gtest.h"
#include "RateFramePlayable.h"
#include "Timer.h"

using namespace PJ;
using namespace std;

namespace RateFrameAnimatorTests {
}

using namespace RateFrameAnimatorTests;

TEST(RateFramePlayable, Tests)
{
    RateFramePlayable sut(10, 10.0f);

    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0)));
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(1.0f)));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f)));
    EXPECT_EQ(2, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
}
