#include "gtest/gtest.h"

#include "OrderedSet.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

TEST(OrderedSet, Contains) {
    OrderedSet<String> sut;

    sut.insert("test");
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(Contains(sut, "test"));

    sut.erase("test");
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(Contains(sut, "test"));

    sut.erase("test");
    EXPECT_EQ(0, sut.size());
}

TEST(OrderedSet, AddOrRemove) {
    OrderedSet<String> sut;

    AddOrRemove(sut, "test", true);
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(Contains(sut, "test"));

    AddOrRemove(sut, "test", false);
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(Contains(sut, "test"));

    AddOrRemove(sut, "test", false);
    EXPECT_EQ(0, sut.size());
}

TEST(OrderedSet, OperatorMinusDifference) {
    OrderedSet<int> set1 { 1, 3, 4, 5 };
    OrderedSet<int> set2 { 3, 4 };
    OrderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
    EXPECT_FALSE(Contains(sut, 3));
    EXPECT_FALSE(Contains(sut, 4));
}

TEST(OrderedSet, OperatorMinusNoDifference) {
    OrderedSet<int> set1 { 1, 5 };
    OrderedSet<int> set2 { 9, 11 };
    OrderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
}
