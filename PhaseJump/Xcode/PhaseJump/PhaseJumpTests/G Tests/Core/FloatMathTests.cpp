#include "gtest/gtest.h"
#include "FloatMath.h"

using namespace PJ;
using namespace std;

namespace FloatMathTests {
}

using namespace FloatMathTests;

TEST(FloatMath, Sqrt)
{
    EXPECT_NEAR(3.0f, FloatMath::Sqrt(9.0f), std::numeric_limits<float>::epsilon());
}
