#include "gtest/gtest.h"
#include "PublishedValue.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace PublishedValueTests {
}

using namespace PublishedValueTests;

TEST(PublishedValue, Lifecycle)
{
    auto sut = MAKE<PublishedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());

    VectorList<int> values;

    auto subscription = sut->Receive([&values](int const& value) {
        values.push_back(value);
    });

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());
    sut->SetValue(2);
    EXPECT_EQ(2, sut->Value());

    VectorList<int> expectedValues{12, 2};
    EXPECT_EQ(expectedValues, values);
}

TEST(PublishedValue, OperatorAssign)
{
    auto sut = MAKE<PublishedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    VectorList<int> values;

    auto subscription = sut->Receive([&values](int const& value) {
        values.push_back(value);
    });

    *sut = 12;
    EXPECT_EQ(12, sut->Value());
    
    VectorList<int> expectedValues{10, 12};
    EXPECT_EQ(expectedValues, values);
}
