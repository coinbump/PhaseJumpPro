#include "gtest/gtest.h"

#include "StringUtils.h"
#include "VectorList.h"

using namespace std;
using namespace PJ;

TEST(VectorListString, TestJoinStrings3) {
    VectorList<String> strings;
    Add(strings, "a");
    Add(strings, "b");
    Add(strings, "c");

    auto sut = Joined(strings, ",");
    EXPECT_EQ("a,b,c", sut);
}
