#include "gtest/gtest.h"

#include "OrderedSet.h"
#include "StringUtils.h"
#include "SetUtils.h"

using namespace std;
using namespace PJ;

TEST(SetUtils, UnorderedRemove) {
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

TEST(SetUtils, UnorderedAddOrRemove) {
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

TEST(SetUtils, UnorderedOperatorMinusDifference) {
    UnorderedSet<int> set1 { 1, 3, 4, 5 };
    UnorderedSet<int> set2 { 3, 4 };
    UnorderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
    EXPECT_FALSE(Contains(sut, 3));
    EXPECT_FALSE(Contains(sut, 4));
}

TEST(SetUtils, UnorderedOperatorMinusNoDifference) {
    UnorderedSet<int> set1 { 1, 5 };
    UnorderedSet<int> set2 { 9, 11 };
    UnorderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
}

TEST(SetUtils, OrderedContains) {
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

TEST(SetUtils, OrderedAddOrRemove) {
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

TEST(SetUtils, OrderedOperatorMinusDifference) {
    OrderedSet<int> set1 { 1, 3, 4, 5 };
    OrderedSet<int> set2 { 3, 4 };
    OrderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
    EXPECT_FALSE(Contains(sut, 3));
    EXPECT_FALSE(Contains(sut, 4));
}

TEST(SetUtils, OrderedOperatorMinusNoDifference) {
    OrderedSet<int> set1 { 1, 5 };
    OrderedSet<int> set2 { 9, 11 };
    OrderedSet<int> sut = set1 - set2;

    EXPECT_EQ(2, sut.size());
    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_TRUE(Contains(sut, 5));
}

TEST(SetUtils, FilterSetUnordered) {
    UnorderedSet<int> sut;
    sut.insert(1);
    sut.insert(2);
    sut.insert(3);
    sut.insert(4);

    auto values = Filter(sut, [](int value) { return value < 3; });
    EXPECT_EQ(2, values.size());

    EXPECT_TRUE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
}

TEST(SetUtils, FilterSetOrdered) {
    OrderedSet<int> sut;
    sut.insert(1);
    sut.insert(2);
    sut.insert(3);
    sut.insert(4);

    auto values = Filter(sut, [](int value) { return value < 3; });
    EXPECT_EQ(2, values.size());

    EXPECT_TRUE(sut.contains(1));
    EXPECT_TRUE(sut.contains(2));
}

TEST(SetUtils, MapSetUnordered) {
    UnorderedSet<int> sut{ 1, 2, 3 };

    auto values = Map<String>(sut, [](int value) { return MakeString(value); });
    EXPECT_EQ(3, values.size());

    UnorderedSet<String> expectedValues{ "1", "2", "3" };
    EXPECT_EQ(expectedValues, values);
}

TEST(SetUtils, MapSetOrdered) {
    OrderedSet<int> sut{ 1, 2, 3 };

    auto values = Map<String>(sut, [](int value) { return MakeString(value); });
    EXPECT_EQ(3, values.size());

    OrderedSet<String> expectedValues{ "1", "2", "3" };
    EXPECT_EQ(expectedValues, values);
}
