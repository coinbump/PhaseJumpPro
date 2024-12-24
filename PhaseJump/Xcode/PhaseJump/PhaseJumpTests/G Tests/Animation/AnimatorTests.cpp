#include "gtest/gtest.h"
#include "Animator.h"

using namespace PJ;
using namespace std;

namespace AnimatorTests {
}

using namespace AnimatorTests;

TEST(Animator, Animate)
{
    auto testValue = -1.0f;

    auto interpolator = InterpolateFuncs::Ease(InterpolateFuncs::Make<float>(0.0f, 360.0f));
    SetBindingFunc<float> binding = [&testValue](float value) { testValue = value; };
    Animator<float> sut(interpolator,
                        1.0f,
                        binding);

    sut.OnUpdate(TimeSlice(0.5f));
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(180.0f, testValue);
    EXPECT_FALSE(sut.IsFinished());

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());
    EXPECT_EQ(360.0f, testValue);
    EXPECT_TRUE(sut.IsFinished());
}

TEST(Animator, SetProgress)
{
    auto testValue = -1.0f;

    auto interpolator = InterpolateFuncs::Ease(InterpolateFuncs::Make<float>(0.0f, 360.0f));
    SetBindingFunc<float> binding = [&testValue](float value) { testValue = value; };
    Animator<float> sut(interpolator,
                        1.0f,
                        binding);

    sut.SetProgress(0.5f);
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(180.0f, testValue);
    EXPECT_FALSE(sut.IsFinished());

    sut.SetProgress(1.0f);
    EXPECT_EQ(1.0f, sut.Progress());
    EXPECT_EQ(360.0f, testValue);
    EXPECT_TRUE(sut.IsFinished());

    sut.SetProgress(0.5f);
    EXPECT_EQ(0.5f, sut.Progress());
    EXPECT_EQ(180.0f, testValue);
    EXPECT_FALSE(sut.IsFinished());
}

TEST(Animator, CycleOnceForward)
{
    auto testValue = -1.0f;

    Animator<float> sut(
                             InterpolateFuncs::Make(0.0f, 360.0f),
                             1.0f,
                             [&testValue] (float value) { testValue = value; }, AnimationCycleType::Once
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

TEST(Animator, MatchInReverse)
{
    auto testValue = -1.0f;

    Animator<float> sut(
                             InterpolateFuncs::Ease(InterpolateFuncs::Make(0.0f, 360.0f), EaseFuncs::inSquared),
                             1.0f,
                             [&testValue] (float value) { testValue = value; }, AnimationCycleType::PingPong,
                             InterpolateFuncs::Ease(InterpolateFuncs::Make(360.0f, 0.0f), EaseFuncs::inSquared));

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(0.2f));
    EXPECT_EQ(0.8f, sut.Progress());
    EXPECT_NEAR(360.0f * (1.0f - (0.2f * 0.2f)), testValue, 0.0001f);
}

TEST(Animator, RewindInReverse)
{
    auto testValue = -1.0f;

    Animator<float> sut(
                             InterpolateFuncs::Ease(InterpolateFuncs::Make(0.0f, 360.0f), EaseFuncs::inSquared),
                             1.0f,
                             [&testValue] (float value) { testValue = value; }, AnimationCycleType::PingPong
                             );

    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(0.2f));
    EXPECT_EQ(0.8f, sut.Progress());
    EXPECT_NEAR(360.0f * 0.8f * 0.8f, testValue, 0.0001f);
}
