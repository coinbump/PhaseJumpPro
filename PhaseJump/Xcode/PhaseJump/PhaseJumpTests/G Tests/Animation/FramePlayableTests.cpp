#include "gtest/gtest.h"
#include "FramePlayable.h"

using namespace PJ;
using namespace std;

namespace FramePlayableTests {
}

using namespace FramePlayableTests;

TEST(FramePlayable, Tests)
{
    FramePlayable sut(10, 10.0f, AnimationCycleType::Loop);

    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0)));
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(1.0f)));
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
    EXPECT_EQ(2, sut.FrameForUpdate(TimeSlice(1.0f/10.0f)));
}

TEST(FramePlayable, DivideByZero)
{
    FramePlayable sut(0, 0, AnimationCycleType::Once);

    EXPECT_EQ(0, sut.FrameTime());
    EXPECT_EQ(0, sut.Duration());
}

TEST(FramePlayable, FrameRate)
{
    FramePlayable sut(48, 24, AnimationCycleType::Once);

    EXPECT_EQ(24, sut.FrameRate());
    EXPECT_EQ(2, sut.Duration());

    sut.SetFrameRate(12);
    EXPECT_EQ(4, sut.Duration());
    EXPECT_EQ(12, sut.FrameRate());
}
