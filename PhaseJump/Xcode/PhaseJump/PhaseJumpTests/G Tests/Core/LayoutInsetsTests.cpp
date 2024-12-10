#include "gtest/gtest.h"
#include "LayoutInsets.h"

using namespace PJ;
using namespace std;

namespace LayoutInsetsTests {
}

using namespace LayoutInsetsTests;

TEST(LayoutInsets, Inits)
{
    LayoutInsets sut = LayoutInsets::Horizontal(10);
    
    EXPECT_EQ(10, sut.leading);
    EXPECT_EQ(10, sut.trailing);
    EXPECT_EQ(0, sut.top);
    EXPECT_EQ(0, sut.bottom);
    
    sut = LayoutInsets::Vertical(10);
    EXPECT_EQ(0, sut.leading);
    EXPECT_EQ(0, sut.trailing);
    EXPECT_EQ(10, sut.top);
    EXPECT_EQ(10, sut.bottom);
}

TEST(LayoutInsets, Sums)
{
    LayoutInsets sut = { .leading = 5, .trailing = 10, .top = 4, .bottom = 8 };
    EXPECT_EQ(15, sut.HorizontalSum());
    EXPECT_EQ(12, sut.VerticalSum());
    EXPECT_EQ(Vector2(15, 12), sut.Sum());
}

TEST(LayoutInsets, AddOperators)
{
    LayoutInsets sut = { .leading = 5, .trailing = 10, .top = 4, .bottom = 8 };
    LayoutInsets rhs = { .leading = 10, .trailing = 2, .top = 5, .bottom = 9 };
    LayoutInsets expectedValue = { .leading = 5 + 10, .trailing = 10 + 2, .top = 4 + 5, .bottom = 8 + 9 };
    
    EXPECT_EQ(expectedValue, sut + rhs);
    
    sut += rhs;
    EXPECT_EQ(expectedValue, sut);
}
