#include "gtest/gtest.h"

#include "VectorList.h"

using namespace std;
using namespace PJ;

TEST(VectorListString, TestJoinStrings3) {
    VectorList<String> strings;
    strings.Add("a");
    strings.Add("b");
    strings.Add("c");

    auto sut = strings.Joined(",");
    EXPECT_EQ("a,b,c", sut);
}
