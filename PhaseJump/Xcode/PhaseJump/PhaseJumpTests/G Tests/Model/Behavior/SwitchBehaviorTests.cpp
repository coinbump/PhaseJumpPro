#include "gtest/gtest.h"
#include "SwitchBehavior.h"

using namespace PJ;
using namespace std;

namespace SwitchBehaviorTests {
}

using namespace SwitchBehaviorTests;

TEST(SwitchBehavior, ChooseFunc) {
    SwitchBehavior sut([](auto& behavior) {
        return 1;
    });

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    BehaviorNode* childPtr1 = child1.get();
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_TRUE(childPtr2->IsRunning());
}

TEST(SwitchBehavior, InvalidChooseFunc) {
    SwitchBehavior sut([](auto& behavior) {
        return 100;
    });

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    BehaviorNode* childPtr1 = child1.get();
    sut.tree.Add(child1);

    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_FALSE(childPtr1->IsRunning());
    EXPECT_FALSE(childPtr2->IsRunning());
}

TEST(SwitchBehavior, InvalidRun) {
    SwitchBehavior sut([](auto& behavior) {
        return 100;
    });

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}

TEST(SwitchBehavior, AllChildrenFinish) {
    SwitchBehavior sut([](auto& behavior) {
        return 0;
    });

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->onUpdateFunc = [](auto& updatable, auto time) {
        return FinishType::Finish;
    };
    sut.tree.Add(child1);

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
}

TEST(SwitchBehavior, OneChildFinish) {
    SwitchBehavior sut([](auto& behavior) {
        return 0;
    });

    UP<BehaviorNode> child1 = NEW<BehaviorNode>();
    BehaviorNode* childPtr1 = child1.get();
    childPtr1->onUpdateFunc = [](auto& updatable, auto time) {
        return FinishType::Finish;
    };
    sut.tree.Add(child1);
    
    UP<BehaviorNode> child2 = NEW<BehaviorNode>();
    BehaviorNode* childPtr2 = child2.get();
    sut.tree.Add(child2);

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Success, sut.State());
    EXPECT_FALSE(childPtr1->IsRunning());
}

