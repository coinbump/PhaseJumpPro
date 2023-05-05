#include "gtest/gtest.h"
#include "StateMachine.h"

using namespace PJ;
using namespace std;

namespace StateMachineTests {
    enum class TestEnum
    {
        Invalid,
        Test1,
        Test2
    };

    class TestStateMachine : public StateMachine<TestEnum>
    {
    public:
        using Base = StateMachine<TestEnum>;

        int test1Count = 0;
        int test2Count = 0;

        void OnStateChange(TestEnum newState) override
        {
            Base::OnStateChange(newState);

            switch (newState) {
                case TestEnum::Test1:
                    test1Count++;
                    break;
                case TestEnum::Test2:
                    test2Count++;
                    break;
                case TestEnum::Invalid:
                    break;
            }
        }
    };
}

using namespace StateMachineTests;

TEST(StateMachine, Test_StateMachine)
{
    auto sut = MAKE<TestStateMachine>();
    sut->SetState(TestEnum::Test2);
    EXPECT_EQ(1, sut->test2Count);
    sut->SetIsLocked(true);

    sut->SetState(TestEnum::Test1);
    EXPECT_EQ(0, sut->test1Count);
    sut->SetIsLocked(false);
    sut->SetState(TestEnum::Test1);
    EXPECT_EQ(1, sut->test1Count);

    sut->SetState(TestEnum::Test2);
    EXPECT_EQ(TestEnum::Test1, sut->PrevState());
}

TEST(StateMachine, Test_StateMachine_Graph)
{
    auto sut = MAKE<TestStateMachine>();
    auto invalidNode = sut->AddState(TestEnum::Invalid);
    EXPECT_EQ(invalidNode, sut->NodeForState(TestEnum::Invalid));
    EXPECT_EQ(1, sut->nodes.Count());

    VectorList<String> inputs1;
    inputs1.Add("test1");
    sut->ConnectStates(TestEnum::Invalid, inputs1, TestEnum::Test1);

    VectorList<String> inputs2;
    inputs2.Add("test2");
    sut->ConnectStates(TestEnum::Test1, inputs2, TestEnum::Test2);

    EXPECT_EQ(3, sut->nodes.Count());
    sut->SetState(TestEnum::Invalid);
    sut->OnInput("test2");
    EXPECT_EQ(TestEnum::Invalid, sut->State());

    sut->OnInput("test1");
    EXPECT_EQ(TestEnum::Test1, sut->State());

    sut->OnInput("test2");
    EXPECT_EQ(TestEnum::Test2, sut->State());
}
