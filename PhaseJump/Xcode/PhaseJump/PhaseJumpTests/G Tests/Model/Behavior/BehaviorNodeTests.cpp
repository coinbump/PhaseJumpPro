#include "gtest/gtest.h"
#include "BehaviorNode.h"

using namespace PJ;
using namespace std;

namespace BehaviorNodeTests {
}

using namespace BehaviorNodeTests;

TEST(BehaviorNode, Init) {
    BehaviorNode sut;

    EXPECT_EQ(BehaviorState::Initial, sut.State());
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.finishState);
}

TEST(BehaviorNode, AddChild) {
    BehaviorNode sut;

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);

    EXPECT_EQ(1, sut.tree.ChildCount());
    EXPECT_EQ(childPtr, sut.Target());
}

TEST(BehaviorNode, ChildSuccessCount) {
    BehaviorNode sut;

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);
    childPtr->finishState = BehaviorState::Success;

    EXPECT_EQ(0, sut.ChildSuccessCount());
    childPtr->Finish();
    EXPECT_EQ(1, sut.ChildSuccessCount());
}

TEST(BehaviorNode, ChildFailureCount) {
    BehaviorNode sut;

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);
    childPtr->finishState = BehaviorState::Failure;

    EXPECT_EQ(0, sut.ChildFailureCount());
    childPtr->Finish();
    EXPECT_EQ(1, sut.ChildFailureCount());
}

TEST(BehaviorNode, OnRun) {
    BehaviorNode sut;

    float parentRunTime{};
    sut.updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        parentRunTime += time.delta;

        return FinishType::Continue;
    };

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);

    int runCount{};
    float runTime{};
    childPtr->onRunFunc = [&](auto& behavior) {
        runCount++;
    };
    childPtr->updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        runTime += time.delta;

        return FinishType::Continue;
    };

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_TRUE(childPtr->IsRunning());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(3, runTime);
    EXPECT_EQ(3, parentRunTime);
}

TEST(BehaviorNode, OnFinish) {
    BehaviorNode sut;

    float parentRunTime{};
    float targetFinishCount{};
    sut.updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        parentRunTime += time.delta;

        return FinishType::Continue;
    };
    sut.onTargetFinishFunc = [&](auto& behavior, auto& target) {
        targetFinishCount++;
    };

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);

    int runCount{};
    float runTime{};
    int finishCount{};
    childPtr->onRunFunc = [&](auto& behavior) {
        runCount++;
    };
    childPtr->updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        GUARDR(runTime == 0, FinishType::Finish)

        runTime += time.delta;

        return FinishType::Continue;
    };
    Updatable::Func onFinishFunc = [&](auto& behavior) {
        finishCount++;
    };
    Override(childPtr->updatable.onFinishFunc, onFinishFunc);
    childPtr->finishState = BehaviorState::Failure;

    sut.OnUpdate({3});
    sut.OnUpdate({2});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_EQ(BehaviorState::Failure, childPtr->State());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(1, finishCount);
    EXPECT_EQ(1, targetFinishCount);
    EXPECT_EQ(3, runTime);
    EXPECT_EQ(5, parentRunTime);
}
