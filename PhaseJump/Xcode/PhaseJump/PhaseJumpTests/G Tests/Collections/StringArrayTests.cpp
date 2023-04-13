#include "gtest/gtest.h"

#include "StringVectorList.h"

using namespace std;
using namespace PJ;

TEST(StringVectorList, TestJoinStrings3) {
    StringVectorList strings;
    strings.Add("a");
    strings.Add("b");
    strings.Add("c");

    auto sut = strings.Joined(",");
    EXPECT_EQ("a,b,c", sut);
}
