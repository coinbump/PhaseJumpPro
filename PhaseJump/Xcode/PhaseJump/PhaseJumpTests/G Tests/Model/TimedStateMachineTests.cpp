#include "gtest/gtest.h"
#include "TimedStateMachine.h"

using namespace PJ;
using namespace std;

namespace TimedStateMachineTests {
    enum class TestEnum
    {
        Invalid,
        Test1,
        Test2
    };

    class TestStateMachine : public TimedStateMachine<TestEnum>
    {
    public:
        using Base = TimedStateMachine<TestEnum>;

        int test1Count;
        int test2Count;
        int finishedCount;

        void OnStateFinish() override
        {
            Base::OnStateFinish();

            finishedCount++;
        }

        void OnStateChange(TestEnum newState) override
        {
            Base::OnStateChange(newState);

            switch (newState)
            {
                case TestEnum::Test1:
                    test1Count++;
                    break;
                case TestEnum::Test2:
                    test2Count++;
                    break;
                default:
                    break;
            }
        }
    };
}

using namespace TimedStateMachineTests;

TEST(TimedStateMachine, Test_TimedStateMachine)
{
    auto test = MAKE<TestStateMachine>();
    test->SetState(TestEnum::Test2);
    EXPECT_EQ(1, test->test2Count);
    test->SetIsLocked(true);

    test->SetState(TestEnum::Test1);
    EXPECT_EQ(0, test->test1Count);
    test->SetIsLocked(false);
    test->SetState(TestEnum::Test1);
    EXPECT_EQ(1, test->test1Count);

    test->SetStateDuration(1.0f);
    test->OnUpdate(TimeSlice(1.5f));
    EXPECT_EQ(test->finishedCount, 1);
    test->ResetStateTimer();
    test->OnUpdate(TimeSlice(.5f));
    EXPECT_NEAR(.5f, test->StateProgress(), .001f);
    test->OnUpdate(TimeSlice(.6f));
    EXPECT_EQ(test->finishedCount, 2);

    test->SetState(TestEnum::Test2);
    EXPECT_EQ(TestEnum::Test1, test->PrevState());
}
