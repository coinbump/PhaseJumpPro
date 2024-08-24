#include "gtest/gtest.h"
#include "StringUtils.h"

using namespace PJ;
using namespace std;

namespace StringUtilsTests {
}

using namespace StringUtilsTests;

TEST(StringUtils, U32CharToString)
{
    // FUTURE: update when Unicode support is added
    EXPECT_EQ("a", U32CharToString('a'));
}

TEST(StringUtils, ToU32String)
{
    std::u32string u32 = U"a";

    // FUTURE: update when Unicode support is added
    EXPECT_EQ(u32, ToU32String("a"));
}
