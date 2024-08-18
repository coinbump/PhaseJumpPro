#include "gtest/gtest.h"
#include "ComposeTimedStateMachine.h"

using namespace PJ;
using namespace std;

namespace ComposeTimedStateMachineTests {
    enum class StateType
    {
        Invalid, Left, Right
    };

    class TestOwner : public SomeTimedStateMachineOwner<StateType> {
    public:
        virtual void OnStateChange(TimedStateMachine<StateType>& inStateMachine) {}
        virtual void OnStateFinish(TimedStateMachine<StateType>& inStateMachine) {}
    };

    class TestObject : public ComposeTimedStateMachine<StateType>
    {
    public:
        using Base = ComposeTimedStateMachine<StateType>;

        int stateChangeCount = 0;
        int stateFinishCount = 0;

        TestObject(SomeTimedStateMachineOwner<StateType>& owner) : Base(owner) {
        }

        void OnStateChange(StateType nextState) override
        {
            Base::OnStateChange(nextState);
            stateChangeCount++;
        }

        void OnStateFinish() override
        {
            Base::OnStateFinish();
            stateFinishCount++;
        }
    };
}

using namespace ComposeTimedStateMachineTests;

TEST(ComposeTimedStateMachine, StateChange)
{
    TestOwner owner;
    
    auto sut = MAKE<TestObject>(owner);

    EXPECT_EQ(0, sut->stateChangeCount);
    sut->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
    sut->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
    sut->SetState(StateType::Left);
    EXPECT_EQ(2, sut->stateChangeCount);
}

TEST(ComposeTimedStateMachine, StateFinish)
{
    TestOwner owner;
    auto sut = MAKE<TestObject>(owner);

    sut->SetState(StateType::Right);
    EXPECT_EQ(0, sut->stateFinishCount);
    sut->SetStateDuration(1.0f);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);
}

TEST(ComposeTimedStateMachine, Configure) {
    TestOwner owner;
    auto sut = MAKE<TestObject>(owner);

    EXPECT_EQ(&owner, &sut->owner);

    EXPECT_EQ(0, sut->stateChangeCount);
    sut->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
}
