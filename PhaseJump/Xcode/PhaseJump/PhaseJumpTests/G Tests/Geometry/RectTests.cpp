#include "gtest/gtest.h"
#include "Rect.h"

using namespace PJ;
using namespace std;

namespace RectTests {
}

using namespace RectTests;

TEST(RectInt, Getters)
{
    RectInt sut({0, 0}, {1, 1});
    
    EXPECT_EQ(0, sut.Left());
    EXPECT_EQ(0, sut.Top());
    EXPECT_EQ(0, sut.Right());
    EXPECT_EQ(0, sut.Bottom());
}
