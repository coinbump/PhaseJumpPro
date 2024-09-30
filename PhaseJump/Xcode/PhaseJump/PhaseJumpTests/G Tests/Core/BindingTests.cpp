#include "gtest/gtest.h"

#include "Binding.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

namespace BindingTests {
}


using namespace BindingTests;

TEST(Binding, GetSet)
{
    int value = 5;
    auto sut = Binding<int>([&]() { return value; }, [&](int newValue) { value = newValue; });

    EXPECT_EQ(5, sut.Value());
    
    sut.SetValue(10);
    EXPECT_EQ(10, sut.Value());
}

TEST(Binding, Operators)
{
    int value = 5;
    auto sut = Binding<int>([&]() { return value; }, [&](int newValue) { value = newValue; });

    EXPECT_EQ(5, sut);

    sut = 10;
    EXPECT_EQ(10, sut.Value());
}
