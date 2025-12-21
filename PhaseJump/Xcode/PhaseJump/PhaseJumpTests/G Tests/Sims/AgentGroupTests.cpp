#include "gtest/gtest.h"
#include "AgentGroup.h"

using namespace PJ;
using namespace std;

namespace AgentGroupTests {
    struct TestCore {
        int intValue = 0;
        float timeValue = 0;
        int stepCount = 0;
    };

    using TestAgent = Agent<TestCore>;
}

using namespace AgentGroupTests;

TEST(AgentGroup, Test)
{
    TypeAgentGroup<TestAgent> sut;
    auto first = sut.Add();
    first->onUpdateFunc = [](auto& p, TimeSlice time) {
        p.core.timeValue += time.delta;
    };
    first->onStepFunc = [](auto& p) {
        p.core.stepCount++;
    };
    ASSERT_NE(nullptr, first);
    EXPECT_EQ(&sut, first->group);

    EXPECT_EQ(0, first->core.intValue);
    sut.RunAgentsAction([](auto& agent) {
        agent.core.intValue++;
    });
    EXPECT_EQ(1, first->core.intValue);

    sut.stepTime = 3;

    sut.OnUpdate({3});
    EXPECT_EQ(3, first->core.timeValue);
    EXPECT_EQ(1, first->core.stepCount);
}
