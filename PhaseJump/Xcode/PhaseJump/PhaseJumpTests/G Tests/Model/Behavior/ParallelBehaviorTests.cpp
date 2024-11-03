#include "gtest/gtest.h"
#include "ParallelBehavior.h"
#include "TimedBehavior.h"

using namespace PJ;
using namespace std;

namespace ParallelBehaviorTests {
}

using namespace ParallelBehaviorTests;

TEST(ParallelBehavior, OnFinishAllSuccess) {
    ParallelBehavior sut;

    UP<BehaviorNode> child1 = NEW<TimedBehavior>(1);
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->finishState = BehaviorState::Success;
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<TimedBehavior>(2);
    BehaviorNode* childPtr2 = child2.get();
    childPtr2->finishState = BehaviorState::Success;
    sut.tree.Add(child2);

    sut.OnUpdate({1});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_TRUE(childPtr2->IsRunning());

    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.State());
}

TEST(ParallelBehavior, OnFinishAllFailure) {
    ParallelBehavior sut;

    UP<BehaviorNode> child1 = NEW<TimedBehavior>(1);
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->finishState = BehaviorState::Failure;
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<TimedBehavior>(2);
    BehaviorNode* childPtr2 = child2.get();
    childPtr2->finishState = BehaviorState::Failure;
    sut.tree.Add(child2);

    sut.OnUpdate({1});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_TRUE(childPtr2->IsRunning());

    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}

TEST(ParallelBehavior, OnFinishPartialSuccess) {
    ParallelBehavior sut;
    sut.successThreshold = 1;

    UP<BehaviorNode> child1 = NEW<TimedBehavior>(1);
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->finishState = BehaviorState::Success;
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<TimedBehavior>(2);
    BehaviorNode* childPtr2 = child2.get();
    childPtr2->finishState = BehaviorState::Success;
    sut.tree.Add(child2);

    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.State());
}

TEST(ParallelBehavior, OnFinishPartialFailure) {
    ParallelBehavior sut;
    sut.failureThreshold = 1;

    UP<BehaviorNode> child1 = NEW<TimedBehavior>(1);
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->finishState = BehaviorState::Failure;
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<TimedBehavior>(2);
    BehaviorNode* childPtr2 = child2.get();
    childPtr2->finishState = BehaviorState::Success;
    sut.tree.Add(child2);

    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}

TEST(ParallelBehavior, OnFinishMixed) {
    ParallelBehavior sut;

    UP<BehaviorNode> child1 = NEW<TimedBehavior>(1);
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->finishState = BehaviorState::Failure;
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<TimedBehavior>(2);
    BehaviorNode* childPtr2 = child2.get();
    childPtr2->finishState = BehaviorState::Success;
    sut.tree.Add(child2);

    sut.finishState = BehaviorState::Failure;

    sut.OnUpdate({1});
    sut.OnUpdate({1});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}


