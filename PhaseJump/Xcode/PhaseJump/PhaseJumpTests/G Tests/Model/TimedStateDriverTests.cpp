#include "gtest/gtest.h"
#include "TimedStateDriver.h"

using namespace PJ;
using namespace std;

namespace TimedStateDriverTests {
    enum class TestEnum
    {
        Invalid,
        Test1,
        Test2
    };

    class TestStateDriver : public TimedStateDriver<TestEnum>
    {
    public:
        using Base = TimedStateDriver<TestEnum>;

        int test1Count{};
        int test2Count{};
        int finishedCount{};

        TestStateDriver(StateMachine<TestEnum>& target) : Base(target) {
            onStateFinishFunc = [&](auto& driver) {
                finishedCount++;
            };

            target.OverrideOnStateChangeFunc([&](auto& target) {
                switch (target.State())
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
            });
        }
    };
}

using namespace TimedStateDriverTests;

TEST(TimedStateDriver, Test_TimedStateDriver)
{
    StateMachine<TestEnum> target;
    TestStateDriver sut(target);
    target.SetState(TestEnum::Test2);
    EXPECT_EQ(1, sut.test2Count);
    target.SetIsLocked(true);

    target.SetState(TestEnum::Test1);
    EXPECT_EQ(0, sut.test1Count);
    target.SetIsLocked(false);
    target.SetState(TestEnum::Test1);
    EXPECT_EQ(1, sut.test1Count);

    sut.SetStateDuration(1.0f);
    sut.OnUpdate(TimeSlice(1.5f));
    EXPECT_EQ(sut.finishedCount, 1);
    sut.ResetStateTimer();
    sut.OnUpdate(TimeSlice(.5f));
    EXPECT_NEAR(.5f, sut.StateProgress(), .001f);
    sut.OnUpdate(TimeSlice(.6f));
    EXPECT_EQ(sut.finishedCount, 2);

    target.SetState(TestEnum::Test2);
    EXPECT_EQ(TestEnum::Test1, target.PrevState());
}
