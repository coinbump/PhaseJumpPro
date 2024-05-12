#include "gtest/gtest.h"

#include "_Set.h"
#include "_String.h"

using namespace std;
using namespace PJ;

TEST(Set, Remove) {
    Set<String> sut;

    sut.Add("test");
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(sut.Contains("test"));

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(sut.Contains("test"));

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());
}

TEST(Set, AddOrRemove) {
    Set<String> sut;

    sut.AddOrRemove("test", true);
    EXPECT_EQ(1, sut.size());
    EXPECT_TRUE(sut.Contains("test"));

    sut.AddOrRemove("test", false);
    EXPECT_EQ(0, sut.size());
    EXPECT_FALSE(sut.Contains("test"));

    sut.AddOrRemove("test", false);
    EXPECT_EQ(0, sut.size());
}

TEST(Set, OperatorMinusDifference) {
    Set<String> set1 { 1, 3, 4, 5 };
    Set<String> set2 { 3, 4 };
    Set<String> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(sut.Contains(1));
    EXPECT_TRUE(sut.Contains(5));
    EXPECT_FALSE(sut.Contains(3));
    EXPECT_FALSE(sut.Contains(4));
}

TEST(Set, OperatorMinusNoDifference) {
    Set<String> set1 { 1, 5 };
    Set<String> set2 { 9, 11 };
    Set<String> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(sut.Contains(1));
    EXPECT_TRUE(sut.Contains(5));
}
