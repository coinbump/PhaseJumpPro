#include "gtest/gtest.h"
#include "PlayableTimer.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace TimerTests {
    class TestTimer : public PlayableTimer
    {
    public:
        using Base = PlayableTimer;

        int finishedCount = 0;

        TestTimer(float duration, RunType runType) : Base(duration, runType)
        {
            updatable.onFinishFunc = [this](auto& _) {
                finishedCount++;
            };
        }
    };
}

using namespace TimerTests;

TEST(PlayableTimer, RunOnce) {
    auto timer = PlayableTimer(1.0f, RunType::Once);
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

TEST(PlayableTimer, Repeat) {
    auto timer = TestTimer(1.0f, RunType::Repeat);
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0.3f, timer.State());
    EXPECT_EQ(0.3f, timer.Progress());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0, timer.State());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(1, timer.finishedCount);
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.7f));
    EXPECT_EQ(0.7f, timer.State());
    EXPECT_EQ(0.7f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(0, timer.State());
    EXPECT_EQ(0, timer.Progress());
    EXPECT_EQ(2, timer.finishedCount);
    EXPECT_FALSE(timer.IsFinished());
}

TEST(PlayableTimer, Stop) {
    auto timer = PlayableTimer(1.0f, RunType::Once);
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_EQ(.3f, timer.State());
    EXPECT_EQ(.3f, timer.Progress());
    EXPECT_FALSE(timer.IsFinished());

    timer.Stop();
    EXPECT_TRUE(timer.IsFinished());
}

// TODO: add test for finishFunc

TEST(PlayableTimer, UpdateFuncs) {
    float value = 0;
    auto timer = PlayableTimer(1.0f, RunType::Once);

    timer.updatable.onUpdateFunc = [&](Updatable& _, TimeSlice time) {
        value += time.delta;

        return FinishType::Continue;
    };

    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_NEAR(0.3f, value, std::numeric_limits<float>::epsilon());

    timer.timeScale = 2.0f;
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_NEAR(0.9f, value, std::numeric_limits<float>::epsilon());
}

TEST(PlayableTimer, UpdateFuncsWhenPaused) {
    float value = 0;
    auto timer = PlayableTimer(1.0f, RunType::Once);

    timer.updatable.onUpdateFunc = [&](Updatable& _, TimeSlice time) {
        value += time.delta;

        return FinishType::Continue;
    };

    timer.Pause();
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_NEAR(0, value, std::numeric_limits<float>::epsilon());

    timer.timeScale = 2.0f;
    timer.OnUpdate(TimeSlice(.3f));
    EXPECT_NEAR(0, value, std::numeric_limits<float>::epsilon());
}
