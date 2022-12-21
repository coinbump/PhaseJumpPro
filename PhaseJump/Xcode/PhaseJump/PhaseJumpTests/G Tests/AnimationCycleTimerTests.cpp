#include "gtest/gtest.h"
#include "Animation/AnimationCycleTimer.h"

using namespace PJ;
using namespace std;

namespace AnimationCycleTimerTests {
}

using namespace AnimationCycleTimerTests;

TEST(AnimationCycleTimer, Test_CycleOnceForward)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::Once);

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());

    sut.OnUpdate(TimeSlice(0.3f));

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(.3f, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(0.7f));
    EXPECT_TRUE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());
}

TEST(AnimationCycleTimer, Test_CycleOnceReverse)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::Once);
    sut.SetCycleState(AnimationCycleState::Reverse);
    sut.SetProgress(1.0f);

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(.3f));

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(.7f, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(0.7f));
    EXPECT_TRUE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());
}

TEST(AnimationCycleTimer, Test_CycleLoopForward)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::Loop);

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_FALSE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());

    sut.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(.3f, sut.Progress());
}

TEST(AnimationCycleTimer, Test_CycleLoopReverse)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::Loop);
    sut.SetCycleState(AnimationCycleState::Reverse);
    sut.SetProgress(1.0f);

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_FALSE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(.7f, sut.Progress());
}

TEST(AnimationCycleTimer, Test_CyclePingPongForward)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::PingPong);

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_FALSE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    sut.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(.7f, sut.Progress());
}

TEST(AnimationCycleTimer, Test_CyclePingPongReverse)
{
    AnimationCycleTimer sut(1.0f, AnimationCycleType::PingPong);
    sut.SetCycleState(AnimationCycleState::Reverse);
    sut.SetProgress(1.0f);

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    EXPECT_FALSE(sut.IsFinished());
    sut.OnUpdate(TimeSlice(1.0f));
    EXPECT_FALSE(sut.IsFinished());

    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(0, sut.Progress());

    sut.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(AnimationCycleState::Forward, sut.CycleState());
    EXPECT_EQ(.3f, sut.Progress());
}
