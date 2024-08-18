#include "gtest/gtest.h"
#include "Alignment.h"

using namespace PJ;
using namespace std;

namespace AlignmentTests {
}

using namespace AlignmentTests;

TEST(Alignment, HorizontalCenter)
{
    auto sut = HorizontalAlignment::center;

    EXPECT_EQ(5, sut.aligner->AlignedOrigin(20, 10));
}

TEST(Alignment, HorizontalLeading)
{
    auto sut = HorizontalAlignment::leading;

    EXPECT_EQ(0, sut.aligner->AlignedOrigin(20, 5));
}

TEST(Alignment, HorizontalTrailing)
{
    auto sut = HorizontalAlignment::trailing;

    EXPECT_EQ(15, sut.aligner->AlignedOrigin(20, 5));
}
