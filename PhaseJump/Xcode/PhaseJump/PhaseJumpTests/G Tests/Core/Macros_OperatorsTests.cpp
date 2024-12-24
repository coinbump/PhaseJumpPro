#include "gtest/gtest.h"

#include "Macros_Operators.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

namespace Macros_OperatorsTests {
    struct Sut {
        int value{};
        
        Sut(int value = 0) : value(value) {
        }
        
        MATH_OPERATORS(Sut, value)
    };
}


using namespace Macros_OperatorsTests;

TEST(Macros_Operators, TestMacros_Operators_ForSuccess)
{
    Sut sut;
    EXPECT_EQ(0, sut.value);
    sut += 10;
    EXPECT_EQ(10, sut.value);
    sut -= 5;
    EXPECT_EQ(5, sut.value);
    sut *= 2;
    EXPECT_EQ(10, sut.value);
    sut /= 2;
    EXPECT_EQ(5, sut.value);
}
