#include "gtest/gtest.h"

#include "VectorList.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

TEST(VectorList, Remove) {
    VectorList<String> sut;

    sut.push_back("test");
    EXPECT_EQ(1, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());
}
