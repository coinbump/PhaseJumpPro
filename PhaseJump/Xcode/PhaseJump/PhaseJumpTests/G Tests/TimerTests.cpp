#include "gtest/gtest.h"
#include "Timer.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace TimerTests {
    class TestTimer : public Timer
    {
    public:
        using Base = Timer;

        int finishedCount = 0;

        TestTimer(float duration, SomeTimed::RunType runType) : Base(duration, runType)
        {
        }

        void OnFinish() override
        {
            Base::OnFinish();

            finishedCount++;
        }
    };
}

using namespace TimerTests;

TEST(Timer, RunOnce) {
    auto timer = Timer(1.0f, SomeTimed::RunType::RunOnce);
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(.3f, timer.TimerState());
    EXPECT_EQ(.3f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(1.0f, timer.TimerState());
    EXPECT_EQ(1.0f, timer.Progress());
    EXPECT_TRUE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(1.0f, timer.TimerState());
    EXPECT_EQ(1.0f, timer.Progress());
    EXPECT_TRUE(timer.IsFinished());

    timer.SetProgress(.5f);
    EXPECT_FALSE(timer.IsFinished());
}

TEST(Timer, KeepRunning) {
    auto timer = TestTimer(1.0f, SomeTimed::RunType::KeepRunning);
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0.3f, timer.TimerState());
    EXPECT_EQ(0.3f, timer.Progress());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0, timer.TimerState());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(1, timer.finishedCount);
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0.7f, timer.TimerState());
    EXPECT_EQ(0.7f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0, timer.TimerState());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(2, timer.finishedCount);
    EXPECT_FALSE(timer.IsFinished());
}
