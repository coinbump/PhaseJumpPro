#include "gtest/gtest.h"
#include "GoCore.h"

using namespace PJ;
using namespace std;

namespace GoCoreTests {
    enum class StateType
    {
        Invalid, Left, Right
    };

    class TestListener : public SomeGoStateListener<StateType> {
        virtual void OnStateChange(GoStateMachine<StateType>& inStateMachine) {}
        virtual void OnStateFinish(GoStateMachine<StateType>& inStateMachine) {}
    };

    class TestObject : public GoCore<StateType>
    {
    public:
        int stateChangeCount = 0;
        int stateFinishCount = 0;

    protected:
        void OnStateChange(GoStateMachine<StateType>& inStateMachine) override
        {
            stateChangeCount++;
        }

        void OnStateFinish(GoStateMachine<StateType>& inStateMachine) override
        {
            stateFinishCount++;
        }
    };
}

using namespace GoCoreTests;

TEST(GoCore, StateChange)
{
    auto sut = MAKE<TestObject>();
    sut->Go();

    EXPECT_EQ(0, sut->stateChangeCount);
    sut->sm->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
    sut->sm->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
    sut->sm->SetState(StateType::Left);
    EXPECT_EQ(2, sut->stateChangeCount);
}

TEST(GoCore, StateFinish)
{
    auto sut = MAKE<TestObject>();
    sut->Go();

    sut->sm->SetState(StateType::Right);
    EXPECT_EQ(0, sut->stateFinishCount);
    sut->sm->SetStateDuration(1.0f);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);
}

TEST(GoCore, Configure) {
    auto sut = MAKE<TestObject>();
    auto listener = MAKE<TestListener>();
    sut->Configure(listener);

    EXPECT_EQ(listener, sut->owner.lock());

    EXPECT_EQ(0, sut->stateChangeCount);
    sut->sm->SetState(StateType::Right);
    EXPECT_EQ(1, sut->stateChangeCount);
}
