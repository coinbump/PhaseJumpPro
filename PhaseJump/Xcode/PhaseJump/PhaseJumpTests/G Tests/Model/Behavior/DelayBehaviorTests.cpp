#include "gtest/gtest.h"
#include "DelayBehavior.h"

using namespace PJ;
using namespace std;

namespace DelayBehaviorTests {
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

    class TestDelayBehavior : public DelayBehavior {
    public:
        using Base = DelayBehavior;
        using This = TestDelayBehavior;

        float runTime = 0;

        TestDelayBehavior(float delay = 3) : Base(delay) {
        }

        FinishType OnUpdate(TimeSlice time) override {
            GUARDR(!IsFinished(), FinishType::Finish)
            Base::OnUpdate(time);

            runTime += time.delta;
            return FinishType::Continue;
        }
    };
}

using namespace DelayBehaviorTests;

TEST(DelayBehavior, TestWithChild) {
    TestDelayBehavior sut;

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
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_FALSE(childPtr->IsFinished());
    EXPECT_EQ(BehaviorState::Initial, childPtr->State());
    EXPECT_EQ(0, runCount);
    EXPECT_EQ(0, finishCount);
    EXPECT_EQ(0, targetFinishCount);
    EXPECT_EQ(0, childPtr->runTime);
    EXPECT_EQ(2, sut.runTime);

    sut.OnUpdate({1});
    sut.OnUpdate({1});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_TRUE(childPtr->IsRunning());
    EXPECT_FALSE(childPtr->IsFinished());
    EXPECT_EQ(BehaviorState::Run, childPtr->State());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(1, childPtr->runTime);
    EXPECT_EQ(4, sut.runTime);
}

TEST(DelayBehavior, TestWithoutChild) {
    TestDelayBehavior sut{3};

    float targetFinishCount{};
    sut.onTargetFinishFunc = [&](auto& behavior, auto& target) {
        targetFinishCount++;
    };

    sut.OnUpdate({2});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_EQ(2, sut.runTime);
    EXPECT_EQ(0, targetFinishCount);

    sut.OnUpdate({1});
    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_EQ(3, sut.runTime);
    EXPECT_EQ(0, targetFinishCount);
}

TEST(DelayBehavior, InvalidDelay) {
    TestDelayBehavior sut(0);

    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}
