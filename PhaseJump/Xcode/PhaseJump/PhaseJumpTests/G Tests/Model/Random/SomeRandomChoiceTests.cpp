#include "gtest/gtest.h"
#include "SomeRandomChoice.h"
#include "MockRandom.h"

#include <memory>

using namespace PJ;
using namespace std;

namespace SomeRandomChoiceTests {
}

using namespace SomeRandomChoiceTests;

TEST(WeightedRandom, MinMaxInt)
{
    MockRandom fixedRandom;
    Add(fixedRandom.values, 0.0f);
    Add(fixedRandom.values, 0.5f);
    Add(fixedRandom.values, 1.0f);

    MinMaxIntRandomChoice sut(0, 10);
    EXPECT_EQ(0, sut.Choose(fixedRandom));
    EXPECT_EQ(5, sut.Choose(fixedRandom));
    EXPECT_EQ(10, sut.Choose(fixedRandom));
}

TEST(WeightedRandom, MinMaxFloat)
{
    MockRandom fixedRandom;
    Add(fixedRandom.values, 0.0f);
    Add(fixedRandom.values, 0.5f);
    Add(fixedRandom.values, 1.0f);

    MinMaxFloatRandomChoice sut(0.0f, 10.0f);
    EXPECT_EQ(0, sut.Choose(fixedRandom));
    EXPECT_EQ(5.0f, sut.Choose(fixedRandom));
    EXPECT_EQ(10.0f, sut.Choose(fixedRandom));
}
