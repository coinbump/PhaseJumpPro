#include "gtest/gtest.h"
#include "CycleAnimator.h"

using namespace PJ;
using namespace std;

namespace CycleAnimatorTests {
}

using namespace CycleAnimatorTests;

TEST(CycleAnimator, CycleOnceForward)
{
    auto testValue = -1.0f;

    CycleAnimator<float> sut(
                             MAKE<Interpolator<float>>(0, 360),
                             MAKE<AnimationCycleTimer>(1.0f, AnimationCycleType::Once),
                             MAKE<SetBinding<float>>([&testValue] (float value) { testValue = value; })
                             );

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(180.0f, testValue);
    EXPECT_FALSE(sut.IsFinished());

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());
    EXPECT_EQ(360.0f, testValue);
    EXPECT_TRUE(sut.IsFinished());
}

TEST(CycleAnimator, MatchInReverse)
{
    auto testValue = -1.0f;

    CycleAnimator<float> sut(
                             MAKE<Interpolator<float>>(0, 360),
                             MAKE<AnimationCycleTimer>(1.0f, AnimationCycleType::PingPong),
                             MAKE<SetBinding<float>>([&testValue] (float value) { testValue = value; })
                             );

    sut.reverseType = CycleAnimator<float>::ReverseType::Match;
    sut.interpolator->transform = InterpolateTypes::easeInSquared;

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(0.2f));
    EXPECT_EQ(0.8f, sut.Progress());
    EXPECT_NEAR(360.0f * (1.0f - (0.2f * 0.2f)), testValue, 0.0001f);
}

TEST(CycleAnimator, RewindInReverse)
{
    auto testValue = -1.0f;

    CycleAnimator<float> sut(
                             MAKE<Interpolator<float>>(0, 360),
                             MAKE<AnimationCycleTimer>(1.0f, AnimationCycleType::PingPong),
                             MAKE<SetBinding<float>>([&testValue] (float value) { testValue = value; })
                             );

    sut.reverseType = CycleAnimator<float>::ReverseType::Rewind;
    sut.interpolator->transform = InterpolateTypes::easeInSquared;

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(0.2f));
    EXPECT_EQ(0.8f, sut.Progress());
    EXPECT_NEAR(360.0f * 0.8f * 0.8f, testValue, 0.0001f);
}
