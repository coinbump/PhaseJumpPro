#include "gtest/gtest.h"
#include "TriggerMap.h"

using namespace PJ;
using namespace std;

namespace TriggerMapTests {
}

using namespace TriggerMapTests;

TEST(TriggerMap, Test)
{
    TriggerMap<int, int> sut;
    EXPECT_EQ(10, sut.AddAction(10)->action);
    sut.AddAction(10);

    EXPECT_EQ(1, sut.ActionsCount());

    sut.AddTrigger(4, [](int trigger) { return trigger == 2; });
    sut.AddTrigger(10, [](int trigger) { return trigger < 3; });

    EXPECT_EQ(10, sut.FirstActionFor(2).value());
    EXPECT_EQ(10, sut.FirstActionFor(1).value());

    auto actions = sut.ActionsFor(2);
    List<int> expectedValues{10, 4};
    EXPECT_EQ(expectedValues, actions);
}
