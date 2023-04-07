#include "gtest/gtest.h"

#include "StringArray.h"

using namespace std;
using namespace PJ;

TEST(StringArray, TestJoinStrings3) {
    StringArray strings;
    strings.Add("a");
    strings.Add("b");
    strings.Add("c");

    auto sut = strings.Joined(",");
    EXPECT_EQ("a,b,c", sut);
}
