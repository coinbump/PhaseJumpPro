#include "gtest/gtest.h"
#include "TimedBehavior.h"

using namespace PJ;
using namespace std;

namespace TimerBehaviorTests {
    class TestBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = BehaviorNode;

        float runTime = 0;

        FinishType OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            runTime += time.delta;
            return FinishType::Continue;
        }
    };

    class TestTimerBehavior : public TimedBehavior {
    public:
        using Base = TimedBehavior;
        using This = TestTimerBehavior;

        float runTime = 0;

        TestTimerBehavior(float duration = 3) : Base(duration) {
        }

        FinishType OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            runTime += time.delta;
            return FinishType::Continue;
        }
    };
}

using namespace TimerBehaviorTests;

TEST(TimedBehavior, Test) {
    TestTimerBehavior sut(3);

    float targetFinishCount{};
    sut.onTargetFinishFunc = [&](auto& behavior, auto& target) {
        targetFinishCount++;
    };

    UP<BehaviorNode> child = NEW<TestBehavior>();
    TestBehavior* childPtr = static_cast<TestBehavior*>(child.get());
    sut.tree.Add(child);

    int runCount{};
    int finishCount{};
    BehaviorNode::Func onRunFunc = [&](auto& behavior) {
        runCount++;
    };
    Override(childPtr->onRunFunc, onRunFunc);

    Updatable::Func onFinishFunc = [&](auto& behavior) {
        finishCount++;
    };
    Override(childPtr->updatable.onFinishFunc, onFinishFunc);
    childPtr->finishState = BehaviorState::Failure;

    sut.OnUpdate({2});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_TRUE(childPtr->IsRunning());
    EXPECT_FALSE(childPtr->IsFinished());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(0, finishCount);
    EXPECT_EQ(0, targetFinishCount);
    EXPECT_EQ(2, childPtr->runTime);
    EXPECT_EQ(2, sut.runTime);

    sut.OnUpdate({1});
    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_TRUE(childPtr->IsFinished());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(3, childPtr->runTime);
    EXPECT_EQ(4, sut.runTime);
}

TEST(TimedBehavior, InvalidDelay) {
    TestTimerBehavior sut(0);

    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}
