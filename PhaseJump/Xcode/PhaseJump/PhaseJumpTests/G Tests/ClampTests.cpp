#include "gtest/gtest.h"
#include "TransformFunc.h"

using namespace PJ;
using namespace std;

namespace ClampTests {
}

using namespace ClampTests;

TEST(FloatClamp, Test)
{
    auto clamp = TransformFuncs::Clamp<float>(0, 3.0f);
    EXPECT_EQ(clamp(-1.0f), 0);
    EXPECT_EQ(clamp(0), 0);
    EXPECT_EQ(clamp(1.0f), 1.0f);
    EXPECT_EQ(clamp(5.0f), 3.0f);
}

TEST(IntClamp, Test)
{
    auto clamp = TransformFuncs::Clamp<int>(0, 3);
    EXPECT_EQ(clamp(-1), 0);
    EXPECT_EQ(clamp(0), 0);
    EXPECT_EQ(clamp(1), 1);
    EXPECT_EQ(clamp(5), 3);
}
