#include "gtest/gtest.h"

#include "Binding.h"
#include "_String.h"

using namespace std;
using namespace PJ;

namespace BindingTests {
}


using namespace BindingTests;

TEST(Binding, GetSet)
{
    SP<int> value = MAKE<int>();
    *value = 5;
    auto sut = Binding<int>([=]() { return *value; }, [=](int newValue) { *value = newValue; });

    EXPECT_EQ(5, sut.Value());
    
    sut.SetValue(10);
    EXPECT_EQ(10, sut.Value());
}

//
//TEST(Binding, TestBinding_ForFailure)
//{
//    auto sut = Binding<int, String>("test");
//    EXPECT_EQ(BindingType::Failure, sut.Type());
//    EXPECT_EQ("test", sut.FailureValue());
//}
