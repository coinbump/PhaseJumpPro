#include "gtest/gtest.h"
#include "ValueTargetGoal.h"

using namespace PJ;
using namespace std;

namespace ValueTargetGoalTests {
}

using namespace ValueTargetGoalTests;

TEST(ValueTargetGoal, ValueTargetGoal)
{
    auto sut = MAKE<ValueTargetGoal>("", 2);

    EXPECT_EQ(0, sut->Progress());

    sut->Increment();
    EXPECT_FALSE(sut->IsComplete());
    EXPECT_EQ(1, sut->Value());
    EXPECT_EQ(0.5f, sut->Progress());

    sut->Increment();
    EXPECT_TRUE(sut->IsComplete());
    EXPECT_EQ(1.0f, sut->Progress());
}
