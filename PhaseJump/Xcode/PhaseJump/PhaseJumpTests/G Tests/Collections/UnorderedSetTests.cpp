#include "gtest/gtest.h"

#include "UnorderedSet.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

TEST(UnorderedSet, Remove) {
    UnorderedSet<String> sut;

    sut.insert("test");
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(Contains(sut, "test"));

    sut.erase("test");
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(Contains(sut, "test"));

    sut.erase("test");
    EXPECT_EQ(0, sut.size());
}

TEST(UnorderedSet, AddOrRemove) {
    UnorderedSet<String> sut;

    AddOrRemove(sut, "test", true);
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(Contains(sut, "test"));

    AddOrRemove(sut, "test", false);
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(Contains(sut, "test"));

    AddOrRemove(sut, "test", false);
    EXPECT_EQ(0, sut.size());
}

// TODO:
//TEST(UnorderedSet, OperatorMinusDifference) {
//    UnorderedSet<String> set1 { 1, 3, 4, 5 };
//    UnorderedSet<String> set2 { 3, 4 };
//    UnorderedSet<String> sut = set1 - set2;
//
//    EXPECT_EQ(2, sut.size());
//    EXPECT_TRUE(Contains(sut, 1));
//    EXPECT_TRUE(Contains(sut, 5));
//    EXPECT_FALSE(Contains(sut, 3));
//    EXPECT_FALSE(Contains(sut, 4));
//}
//
//TEST(UnorderedSet, OperatorMinusNoDifference) {
//    UnorderedSet<String> set1 { 1, 5 };
//    UnorderedSet<String> set2 { 9, 11 };
//    UnorderedSet<String> sut = set1 - set2;
//
//    EXPECT_EQ(2, sut.size());
//    EXPECT_TRUE(Contains(sut, 1));
//    EXPECT_TRUE(Contains(sut, 5));
//}
