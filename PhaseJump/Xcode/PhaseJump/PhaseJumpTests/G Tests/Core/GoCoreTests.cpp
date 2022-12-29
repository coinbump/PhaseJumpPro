#include "gtest/gtest.h"
#include "GoCore.h"

using namespace PJ;
using namespace std;

namespace GoCoreTests {
    enum class StateType
    {
        Invalid, Left, Right
    };

    class TestObject : public GoCore<StateType>
    {
    public:
        int stateChangeCount = 0;
        int stateFinishCount = 0;

    protected:
        void OnStateChange(GoStateMachinePtr inStateMachine) override
        {
            stateChangeCount++;
        }

        void OnStateFinish(GoStateMachinePtr inStateMachine) override
        {
            stateFinishCount++;
        }
    };
}

using namespace GoCoreTests;

TEST(GoCore, StateChange)
{
    auto sut = make_shared<TestObject>();
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
    auto sut = make_shared<TestObject>();
    sut->Go();

    sut->sm->SetState(StateType::Right);
    EXPECT_EQ(0, sut->stateFinishCount);
    sut->sm->SetStateDuration(1.0f);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);

    sut->OnUpdate(TimeSlice(1.0f));
    EXPECT_EQ(1, sut->stateFinishCount);
}
