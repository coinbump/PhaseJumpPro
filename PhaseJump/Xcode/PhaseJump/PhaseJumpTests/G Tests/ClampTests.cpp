#include "gtest/gtest.h"
#include "Types/FloatClamp.h"
#include "Types/IntClamp.h"

using namespace PJ;
using namespace std;

namespace ClampTests {
}

using namespace ClampTests;

TEST(FloatClamp, Test)
{
    FloatClamp clamp(0, 3.0f);
    EXPECT_EQ(clamp.Transform(-1.0f), 0);
    EXPECT_EQ(clamp.Transform(0), 0);
    EXPECT_EQ(clamp.Transform(1.0f), 1.0f);
    EXPECT_EQ(clamp.Transform(5.0f), 3.0f);
}

TEST(IntClamp, Test)
{
    IntClamp clamp(0, 3);
    EXPECT_EQ(clamp.Transform(-1), 0);
    EXPECT_EQ(clamp.Transform(0), 0);
    EXPECT_EQ(clamp.Transform(1), 1);
    EXPECT_EQ(clamp.Transform(5), 3);
}
