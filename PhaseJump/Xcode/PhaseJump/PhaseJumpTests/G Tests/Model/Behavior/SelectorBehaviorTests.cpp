#include "gtest/gtest.h"
#include "SelectorBehavior.h"

using namespace PJ;
using namespace std;

namespace SelectorBehaviorTests {
}

using namespace SelectorBehaviorTests;

TEST(SelectorBehavior, FinishSuccess) {
    SelectorBehavior sut;

    float parentRunTime{};
    sut.updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        parentRunTime += time.delta;

        return FinishType::Continue;
    };

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    float runTime1{};
    child1->finishState = BehaviorState::Success;
    child1->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime1 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr1 = child1.get();
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    float runTime2{};
    child2->finishState = BehaviorState::Success;
    child2->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime2 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.State());
    EXPECT_EQ(3, runTime1);
    EXPECT_EQ(BehaviorState::Success, childPtr1->State());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(0, runTime2);
    EXPECT_EQ(BehaviorState::Initial, childPtr2->State());
}

TEST(SelectorBehavior, FinishFailureFirst) {
    SelectorBehavior sut;

    float parentRunTime{};
    sut.updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        parentRunTime += time.delta;

        return FinishType::Continue;
    };

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    float runTime1{};
    child1->finishState = BehaviorState::Failure;
    child1->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime1 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr1 = child1.get();
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    float runTime2{};
    child2->finishState = BehaviorState::Success;
    child2->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime2 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(BehaviorState::Run, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_EQ(3, runTime1);
    EXPECT_EQ(BehaviorState::Failure, childPtr1->State());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(0, runTime2);
    EXPECT_EQ(BehaviorState::Initial, childPtr2->State());

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_EQ(BehaviorState::Success, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_EQ(3, runTime1);
    EXPECT_EQ(BehaviorState::Failure, childPtr1->State());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(3, runTime2);
    EXPECT_EQ(BehaviorState::Success, childPtr2->State());
}

TEST(SelectorBehavior, FinishFailureAll) {
    SelectorBehavior sut;

    float parentRunTime{};
    sut.updatable.onUpdateFunc = [&](auto& updatable, TimeSlice time) {
        parentRunTime += time.delta;

        return FinishType::Continue;
    };

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    float runTime1{};
    child1->finishState = BehaviorState::Failure;
    child1->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime1 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr1 = child1.get();
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    float runTime2{};
    child2->finishState = BehaviorState::Failure;
    child2->updatable.onUpdateFunc = [&](auto& updatable, auto time) {
        runTime2 += time.delta;
        return FinishType::Finish;
    };
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(sut.IsFinished());
    EXPECT_EQ(BehaviorState::Run, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_EQ(3, runTime1);
    EXPECT_EQ(BehaviorState::Failure, childPtr1->State());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(0, runTime2);
    EXPECT_EQ(BehaviorState::Initial, childPtr2->State());

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_TRUE(sut.IsFinished());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_EQ(3, runTime1);
    EXPECT_EQ(BehaviorState::Failure, childPtr1->State());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(3, runTime2);
    EXPECT_EQ(BehaviorState::Failure, childPtr2->State());
}

TEST(SelectorBehavior, InvalidRun) {
    SelectorBehavior sut;

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}
