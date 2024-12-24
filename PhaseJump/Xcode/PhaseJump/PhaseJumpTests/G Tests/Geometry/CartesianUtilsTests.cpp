#include "gtest/gtest.h"
#include "CartesianUtils.h"

using namespace PJ;
using namespace std;

namespace CartesianUtilsTests {
}

using namespace CartesianUtilsTests;

TEST(CartesianUtils, CenteredIn)
{
    Rect frame{.origin = {0, 0}, .size = {50, 50}};
    frame = CartesianUtils::CenteredIn(frame, {100, 100});
    
    EXPECT_EQ(Vector2(25 * vecRight, 25 * vecUp), frame.origin);
    EXPECT_EQ(Vector2(50, 50), frame.size);
}
