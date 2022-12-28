#include "gtest/gtest.h"

#include "Array.h"
#include "_String.h"

using namespace std;
using namespace PJ;

TEST(Array, Remove) {
    Array<String> sut;

    sut.Add("test");
    EXPECT_EQ(1, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());
}