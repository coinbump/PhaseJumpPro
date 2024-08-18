#include "gtest/gtest.h"
#include "PublishedValue.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace PublishedValueTests {
}

using namespace PublishedValueTests;

TEST(PublishedValue, Test_Lifecycle)
{
    auto sut = MAKE<PublishedValue<int>>(10);
    EXPECT_EQ(10, sut->Value());

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());

    List<int> values;

    auto subscription = sut->subject.Receive([&values](int const& value) {
        values.Add(value);
    });

    sut->SetValue(12);
    EXPECT_EQ(12, sut->Value());
    sut->SetValue(2);
    EXPECT_EQ(2, sut->Value());

    List<int> expectedValues{12, 2};
    EXPECT_EQ(expectedValues, values);
}
