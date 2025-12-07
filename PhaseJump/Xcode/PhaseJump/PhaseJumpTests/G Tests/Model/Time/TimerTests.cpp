#include "gtest/gtest.h"
#include "Timer.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace TimerTests {
}

using namespace TimerTests;

TEST(Timer, RunOnce) {
    auto timer = Timer({
        .duration = 1.0f,
        .runType = RunType::Once
    });
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(.3f, timer.State());
    EXPECT_EQ(.3f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(1.0f, timer.State());
    EXPECT_EQ(1.0f, timer.Progress());
    EXPECT_TRUE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(1.0f, timer.State());
    EXPECT_EQ(1.0f, timer.Progress());
    EXPECT_TRUE(timer.IsFinished());

    timer.SetProgress(.5f);
    EXPECT_FALSE(timer.IsFinished());
}

TEST(Timer, Repeat) {
    int finishedCount = 0;
    
    auto timer = Timer({
        .duration = 1.0f,
        .runType = RunType::Repeat,
        .onFinishFunc = [&](auto timer) {
            finishedCount++;
        }
    });
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0.3f, timer.State());
    EXPECT_EQ(0.3f, timer.Progress());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0, timer.State());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(1, finishedCount);
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0.7f, timer.State());
    EXPECT_EQ(0.7f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0, timer.State());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(2, finishedCount);
    EXPECT_FALSE(timer.IsFinished());
}

TEST(Timer, Stop) {
    auto timer = Timer(1.0f, RunType::Once);
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(.3f, timer.State());
    EXPECT_EQ(.3f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.Stop();
    EXPECT_TRUE(timer.IsFinished());
}
