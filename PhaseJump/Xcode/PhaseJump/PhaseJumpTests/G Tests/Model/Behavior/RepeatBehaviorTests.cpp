#include "gtest/gtest.h"
#include "RepeatBehavior.h"

using namespace PJ;
using namespace std;

namespace RepeatBehaviorTests {
    class TestBehavior : public BehaviorNode {
    public:
        using Base = BehaviorNode;
        using This = BehaviorNode;

        float runTime = 0;

        FinishType OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            runTime += time.delta;
            Finish();
            
            return FinishType::Finish;
        }
    };

    class TestRepeatBehavior : public RepeatBehavior {
    public:
        using Base = RepeatBehavior;
        using This = TestRepeatBehavior;

        float runTime = 0;

        TestRepeatBehavior(int repeat = 2) : Base(repeat) {
        }

        FinishType OnUpdate(TimeSlice time) override {
            GUARDR(!IsFinished(), FinishType::Finish)
            Base::OnUpdate(time);

            runTime += time.delta;
            return FinishType::Continue;
        }
    };
}

using namespace RepeatBehaviorTests;

TEST(RepeatBehavior, Test) {
    TestRepeatBehavior sut;

    int parentFinishCount{};
    Updatable::Func parentOnFinishFunc = [&](auto& updatable) {
        parentFinishCount++;
    };
    Override(sut.updatable.onFinishFunc, parentOnFinishFunc);

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
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(0, parentFinishCount);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(1, targetFinishCount);
    EXPECT_EQ(2, childPtr->runTime);
    EXPECT_EQ(2, sut.runTime);

    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_TRUE(childPtr->IsFinished());
    EXPECT_EQ(2, runCount);
    EXPECT_EQ(1, parentFinishCount);
    EXPECT_EQ(2, finishCount);
    EXPECT_EQ(2, targetFinishCount);
    EXPECT_EQ(4, childPtr->runTime);
    EXPECT_EQ(4, sut.runTime);

    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_TRUE(childPtr->IsFinished());
    EXPECT_EQ(2, runCount);
    EXPECT_EQ(1, parentFinishCount);
    EXPECT_EQ(2, finishCount);
    EXPECT_EQ(2, targetFinishCount);
    EXPECT_EQ(4, childPtr->runTime);
    EXPECT_EQ(4, sut.runTime);
}

TEST(RepeatBehavior, InvalidRepeat) {
    TestRepeatBehavior sut(0);

    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}

TEST(RepeatBehavior, ResetRoot) {
    TestRepeatBehavior sut;
    sut.onAllChildrenFinishFunc = [](auto& behavior) {
        behavior.Reset();
    };

    int parentFinishCount{};
    Updatable::Func parentOnFinishFunc = [&](auto& updatable) {
        parentFinishCount++;
    };
    Override(sut.updatable.onFinishFunc, parentOnFinishFunc);

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

    // First repeat
    sut.OnUpdate({2});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_FALSE(childPtr->IsFinished());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(0, parentFinishCount);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(1, targetFinishCount);
    EXPECT_EQ(2, childPtr->runTime);
    EXPECT_EQ(2, sut.runTime);

    // Second repeat
    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Initial, sut.State());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_TRUE(childPtr->IsFinished());
    EXPECT_EQ(2, runCount);
    EXPECT_EQ(1, parentFinishCount);
    EXPECT_EQ(2, finishCount);
    EXPECT_EQ(2, targetFinishCount);
    EXPECT_EQ(4, childPtr->runTime);
    EXPECT_EQ(4, sut.runTime);

    // First repeat after root reset
    sut.OnUpdate({2});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_FALSE(childPtr->IsFinished());
    EXPECT_EQ(3, runCount);
    EXPECT_EQ(1, parentFinishCount);
    EXPECT_EQ(3, finishCount);
    EXPECT_EQ(3, targetFinishCount);
    EXPECT_EQ(6, childPtr->runTime);
    EXPECT_EQ(6, sut.runTime);
}
