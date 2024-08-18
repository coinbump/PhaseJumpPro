#include "gtest/gtest.h"
#include "Range.h"

using namespace PJ;
using namespace std;

namespace RangeTests {
}

using namespace RangeTests;

TEST(Range, InclusiveForward)
{
    auto sut = Range<int>::InclusiveRange(3, 10);

    EXPECT_FALSE(sut.IsInside(2));
    EXPECT_TRUE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(9));
    EXPECT_TRUE(sut.IsInside(10));
    EXPECT_FALSE(sut.IsInside(11));
}

TEST(Range, InclusiveReverse)
{
    auto sut = Range<int>::InclusiveRange(10, 3);

    EXPECT_FALSE(sut.IsInside(2));
    EXPECT_TRUE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(9));
    EXPECT_TRUE(sut.IsInside(10));
    EXPECT_FALSE(sut.IsInside(11));
}

TEST(Range, InsideForward)
{
    auto sut = Range<int>::InsideRange(3, 10);

    EXPECT_FALSE(sut.IsInside(2));
    EXPECT_FALSE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(9));
    EXPECT_FALSE(sut.IsInside(10));
    EXPECT_FALSE(sut.IsInside(11));
}

TEST(Range, InsideReverse)
{
    auto sut = Range<int>::InsideRange(10, 3);

    EXPECT_FALSE(sut.IsInside(2));
    EXPECT_FALSE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(9));
    EXPECT_FALSE(sut.IsInside(10));
    EXPECT_FALSE(sut.IsInside(11));
}

TEST(Range, OpenForward)
{
    Range<int> sut(3, 10, Range<int>::Evaluate::Inclusive, Range<int>::Evaluate::Open);

    EXPECT_FALSE(sut.IsInside(2));
    EXPECT_TRUE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(10));
    EXPECT_TRUE(sut.IsInside(11));
}

TEST(Range, OpenReverse)
{
    Range<int> sut(10, 3, Range<int>::Evaluate::Inclusive, Range<int>::Evaluate::Open);

    EXPECT_TRUE(sut.IsInside(2));
    EXPECT_TRUE(sut.IsInside(3));
    EXPECT_TRUE(sut.IsInside(4));
    EXPECT_TRUE(sut.IsInside(10));
    EXPECT_FALSE(sut.IsInside(11));
}
