#include "gtest/gtest.h"
#include "Alignment2D.h"

using namespace PJ;
using namespace std;

namespace AlignmentTests {
}

using namespace AlignmentTests;

TEST(Alignment, HorizontalCenter)
{
    auto sut = AlignFuncs::center;

    EXPECT_EQ(5, sut(20, 10));
}

TEST(Alignment, HorizontalLeading)
{
    auto sut = AlignFuncs::left;

    EXPECT_EQ(0, sut(20, 5));
}

TEST(Alignment, HorizontalTrailing)
{
    auto sut = AlignFuncs::right;

    EXPECT_EQ(15, sut(20, 5));
}
