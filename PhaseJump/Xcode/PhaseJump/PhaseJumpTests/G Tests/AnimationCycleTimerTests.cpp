#include "gtest/gtest.h"
#include "Animation/AnimationCycleTimer.h"

using namespace PJ;
using namespace std;

namespace AnimationCycleTimerTests {
}

using namespace AnimationCycleTimerTests;

TEST(AnimationCycleTimer, CycleOnceForward)
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

TEST(AnimationCycleTimer, CycleOnceReverse)
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

TEST(AnimationCycleTimer, CycleLoopForward)
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

TEST(AnimationCycleTimer, CycleLoopReverse)
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

TEST(AnimationCycleTimer, CyclePingPongForward)
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

TEST(AnimationCycleTimer, CyclePingPongReverse)
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

TEST(AnimationCycleTimer, SetProgressReverse)
{
    AnimationCycleTimer sut(100, AnimationCycleType::PingPong);
    sut.SetCycleState(AnimationCycleState::Reverse);
    sut.SetProgress(1.0f);

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    sut.SetProgress(0.7f);
    EXPECT_EQ(70, sut.PlayTime());
}

TEST(AnimationCycleTimer, SetPlayTimeReverse)
{
    AnimationCycleTimer sut(100, AnimationCycleType::PingPong);
    sut.SetCycleState(AnimationCycleState::Reverse);
    sut.SetProgress(1.0f);

    EXPECT_EQ(AnimationCycleState::Reverse, sut.CycleState());
    EXPECT_EQ(1.0f, sut.Progress());

    sut.SetPlayTime(70);
    EXPECT_EQ(0.7f, sut.Progress());
    EXPECT_EQ(70, sut.PlayTime());
}

TEST(AnimationCycleTimer, OnPlayTimeChangeFunc)
{
    float value{};

    AnimationCycleTimer sut(100, AnimationCycleType::PingPong);
    sut.SetOnPlayTimeChangeFunc([&](auto& playable) {
        value += playable.PlayTime();
    });

    sut.SetPlayTime(30);
    sut.SetPlayTime(10);
    sut.SetPlayTime(5);

    EXPECT_EQ(value, 45);
}

TEST(AnimationCycleTimer, TestIsPlayingAfterPause)
{
    AnimationCycleTimer sut(1, AnimationCycleType::PingPong);
    EXPECT_TRUE(sut.IsPlaying());
    sut.Pause();
    EXPECT_FALSE(sut.IsPlaying());
    EXPECT_TRUE(sut.IsPaused());
}

TEST(AnimationCycleTimer, TestIsPlayingAfterStop)
{
    AnimationCycleTimer sut(1, AnimationCycleType::PingPong);
    EXPECT_TRUE(sut.IsPlaying());
    sut.Stop();
    EXPECT_FALSE(sut.IsPlaying());
    EXPECT_FALSE(sut.IsPaused());
}

TEST(AnimationCycleTimer, TestIsPlayingAfterFinish)
{
    AnimationCycleTimer sut(1, AnimationCycleType::Once);
    EXPECT_TRUE(sut.IsPlaying());
    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsPlaying());
    EXPECT_FALSE(sut.IsPaused());
}

TEST(AnimationCycleTimer, TestResetAfterReverse)
{
    AnimationCycleTimer sut(1, AnimationCycleType::PingPong);
    sut.OnUpdate({1});
    EXPECT_TRUE(sut.IsReversed());
    sut.Reset();
    EXPECT_EQ(0, sut.PlayTime());
    EXPECT_FALSE(sut.IsReversed());
}
