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
    TypeAgentGroup<TestAgent> sut{100, [](auto& item) {
        item.core = {};
    }};
    auto first = sut.Add();
    ASSERT_TRUE(first);
    
    (*first).Get().onUpdateFunc = [](auto& p, TimeSlice time) {
        p.core.timeValue += time.delta;
    };
    (*first).Get().onStepFunc = [](auto& p) {
        p.core.stepCount++;
    };
    ASSERT_TRUE(first);
    EXPECT_EQ(&sut, (*first).Get().group);

    EXPECT_EQ(0, (*first).Get().core.intValue);
    sut.ForAgents([](auto& agent) {
        agent.core.intValue++;
    });
    EXPECT_EQ(1, (*first).Get().core.intValue);

    sut.stepTime = 3;

    sut.OnUpdate({3});
    EXPECT_EQ(3, (*first).Get().core.timeValue);
    EXPECT_EQ(1, (*first).Get().core.stepCount);
}
