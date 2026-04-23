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

TEST(FramePlayable, SetFrameRateRedistributesKeyframeTimes)
{
    // Construction places 10 keyframes at 0, 1/10, 2/10, ..., 9/10 (spacing 0.1s).
    FramePlayable sut(10, 10.0f, AnimationCycleType::Once);

    // Lower the rate: every keyframe should now sit on a 0.2s grid.
    sut.SetFrameRate(5.0f);

    EXPECT_EQ(5.0f, sut.FrameRate());
    EXPECT_FLOAT_EQ(2.0f, sut.Duration()); // 10 frames × 0.2s = 2.0s

    // Frame 0 still plays at t=0; frame 1 now plays at t=0.2 (was 0.1).
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0)));
    EXPECT_EQ(0, sut.FrameForUpdate(TimeSlice(0.1f)));  // still in frame 0 slot
    EXPECT_EQ(1, sut.FrameForUpdate(TimeSlice(0.1f)));  // crosses into frame 1 at t=0.2
}

TEST(FramePlayable, FrameRateBestGuessFromAverageKeyframeInterval)
{
    FramePlayable sut(4, 30.0f, AnimationCycleType::Once);

    EXPECT_FLOAT_EQ(30.0f, sut.FrameRate());
}

TEST(FramePlayable, SetFrameRateIgnoresZeroOrNegative)
{
    FramePlayable sut(10, 10.0f, AnimationCycleType::Once);
    float const duration = sut.Duration();

    sut.SetFrameRate(0);
    EXPECT_EQ(10.0f, sut.FrameRate());
    EXPECT_FLOAT_EQ(duration, sut.Duration());

    sut.SetFrameRate(-5.0f);
    EXPECT_EQ(10.0f, sut.FrameRate());
    EXPECT_FLOAT_EQ(duration, sut.Duration());
}

TEST(FramePlayable, FrameRateFallsBackOnEmptyTrack)
{
    FramePlayable sut(0, 0, AnimationCycleType::Once);

    EXPECT_EQ(0.0f, sut.FrameRate());
}

TEST(FramePlayable, FrameRateRoundTripAfterHalvingFrameRate)
{
    FramePlayable sut(60, 60.0f, AnimationCycleType::Once);
    EXPECT_FLOAT_EQ(60.0f, sut.FrameRate());
    EXPECT_FLOAT_EQ(1.0f, sut.Duration()); // 60 / 60

    sut.SetFrameRate(120.0f);
    EXPECT_FLOAT_EQ(120.0f, sut.FrameRate());
    EXPECT_FLOAT_EQ(0.5f, sut.Duration()); // 60 / 120
}
