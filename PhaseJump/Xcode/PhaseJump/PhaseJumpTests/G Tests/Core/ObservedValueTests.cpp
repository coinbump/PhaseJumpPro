#include "gtest/gtest.h"
#include "ObservedValue.h"
#include "Utils.h"
#include "VectorList.h"

using namespace PJ;
using namespace std;

namespace ObservedValueTests {
}

using namespace ObservedValueTests;

TEST(ObservedValue, Lifecycle)
{
    auto sut = MAKE<ObservedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());

    VectorList<int> values;

    sut->SetOnValueChangeFunc([&](auto value) {
        values.push_back(value);
    });

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());
    sut->SetValue(2);
    EXPECT_EQ(2, sut->Value());

    VectorList<int> expectedValues{12, 2};
    EXPECT_EQ(expectedValues, values);
}

TEST(ObservedValue, OperatorAssign)
{
    auto sut = MAKE<ObservedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    VectorList<int> values;

    sut->SetOnValueChangeFunc([&](auto value) {
        values.push_back(value);
    });

    *sut = 12;
    EXPECT_EQ(12, sut->Value());
    
    VectorList<int> expectedValues{10, 12};
    EXPECT_EQ(expectedValues, values);
}
