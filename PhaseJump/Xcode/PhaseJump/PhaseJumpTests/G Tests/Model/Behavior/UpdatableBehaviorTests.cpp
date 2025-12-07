#include "gtest/gtest.h"
#include "UpdatableBehavior.h"

using namespace PJ;
using namespace std;

namespace UpdatableBehaviorTests {
    class TestUpdatableBehavior : public UpdatableBehavior {
    public:
        using Base = UpdatableBehavior;
        using This = TestUpdatableBehavior;

        float runTime{};

        TestUpdatableBehavior(BuildFunc buildFunc) : Base(buildFunc) {
        }

        FinishType OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            runTime += time.delta;
            return FinishType::Continue;
        }
    };
}

using namespace UpdatableBehaviorTests;

TEST(UpdatableBehavior, Run) {
    float runTime{};

    TestUpdatableBehavior sut([&](auto& behavior) {
        UP<Updatable> updatable = NEW<Updatable>();
        updatable->onUpdateFunc = [&](auto& updatable, auto time) {
            runTime += time.delta;

            return FinishType::Continue;
        };

        return updatable;
    });

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);

    int runCount{};
    childPtr->onRunFunc = [&](auto& behavior) {
        runCount++;
    };

    sut.OnUpdate({3});
    EXPECT_TRUE(sut.IsRunning());
    EXPECT_TRUE(childPtr->IsRunning());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(3, runTime);
    EXPECT_EQ(3, sut.runTime);
}

TEST(UpdatableBehavior, Finish) {
    float runTime{};

    TestUpdatableBehavior sut([&](auto& behavior) {
        UP<Updatable> updatable = NEW<Updatable>();
        updatable->onUpdateFunc = [&](auto& updatable, auto time) {
            runTime += time.delta;

            return FinishType::Finish;
        };

        return updatable;
    });

    UP<BehaviorNode> child = NEW<BehaviorNode>();
    BehaviorNode* childPtr = child.get();
    sut.tree.Add(child);

    int runCount{};
    childPtr->onRunFunc = [&](auto& behavior) {
        runCount++;
    };

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_FALSE(childPtr->IsRunning());
    EXPECT_EQ(1, runCount);
    EXPECT_EQ(3, runTime);
    EXPECT_EQ(3, sut.runTime);
}

TEST(UpdatableBehavior, InvalidRun) {
    TestUpdatableBehavior sut{{}};

    sut.OnUpdate({3});
    EXPECT_FALSE(sut.IsRunning());
    EXPECT_EQ(BehaviorState::Failure, sut.State());
}
