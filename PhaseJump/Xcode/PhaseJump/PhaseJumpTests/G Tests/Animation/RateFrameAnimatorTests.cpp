#include "gtest/gtest.h"
#include "SomeFrameAnimator.h"
#include "Timer.h"

using namespace PJ;
using namespace std;

namespace RateFrameAnimatorTests {
}

using namespace RateFrameAnimatorTests;

TEST(RateFrameAnimator, Tests)
{
    RateFrameAnimator sut(10.0f);

    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0), 10));
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(1.0f), 10));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f/10.0f), 10));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f), 10));
    EXPECT_EQ(2, sut.FrameForUpdate(TimeSlice(1.0f/10.0f), 10));
}
