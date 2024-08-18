#include "gtest/gtest.h"

#include "TimeSlice.h"
#include <memory>

using namespace std;
using namespace PJ;

namespace TimeSliceTests {
}

using namespace TimeSliceTests;

TEST(TimeSlice, MultiplyOperator)
{
    TimeSlice sut(1.0f);
    TimeSlice compare = sut * 3.0f;
    EXPECT_EQ(3.0f, compare.delta);
}
