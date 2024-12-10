#include "gtest/gtest.h"
#include "Tags.h"
#include "StringUtils.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace TagsTests {
    class TestClass {
    };
}

using namespace TagsTests;

TEST(Tags, Any_Cast) {
    std::any value10 = 10;
    EXPECT_EQ(10, std::any_cast<int>(value10));

    EXPECT_ANY_THROW(std::any_cast<float>(value10));
}

TEST(Tags, Remove) {
    Tags sut;
    sut.Set("test", 10);
    EXPECT_EQ(10, sut.SafeValue<int>("test"));
    
    sut.Remove("test");
    EXPECT_EQ(0, sut.SafeValue<int>("test"));
}

TEST(Tags, Values) {
    Tags sut;
    sut.Set("1", 1);
    sut.Set("2", 2.5f);
    sut.Set("3", String("test"));

    EXPECT_TRUE(sut.TypeContains<int>("1"));
    EXPECT_TRUE(sut.TypeContains<float>("2"));
    EXPECT_TRUE(sut.TypeContains<String>("3"));

    EXPECT_EQ(1, sut.SafeValue<int>("1"));
    EXPECT_EQ(2.5f, sut.SafeValue<float>("2"));
    EXPECT_EQ("test", sut.SafeValue<String>("3"));

    EXPECT_EQ(0, sut.SafeValue<int>("X"));
    EXPECT_EQ(0.0f, sut.SafeValue<float>("X"));
    EXPECT_EQ("", sut.SafeValue<String>("X"));
}

TEST(Tags, Value) {
    Tags sut;
    sut.Insert({"1", 1});
    sut.Insert({"2", 2.5f});
    sut.Insert({"3", String("test")});

    EXPECT_EQ(1, sut.Value<int>("1"));
    EXPECT_EQ(2.5f, sut.Value<float>("2"));
    EXPECT_EQ("test", sut.Value<String>("3"));

    EXPECT_EQ(std::nullopt, sut.Value<int>("X"));
    EXPECT_EQ(std::nullopt, sut.Value<float>("X"));
    EXPECT_EQ(std::nullopt, sut.Value<String>("X"));
}

TEST(Tags, SharedPtr) {
    Tags sut;
    sut.Add({"1", MAKE<TestClass>()});
    EXPECT_NE(nullptr, sut.Value<SP<TestClass>>("1"));
}

TEST(Tags, PlusOperator) {
    Tags sut1;
    sut1.Add({"1", String("one")});
    Tags sut2;
    sut2.Add({"2", String("two")});

    auto sut = sut1 + sut2;
    EXPECT_EQ(2, sut.Count());
    EXPECT_EQ("one", sut.Value<String>("1"));
    EXPECT_EQ("two", sut.Value<String>("2"));
}

TEST(Tags, At) {
    Tags sut;
    sut.Insert({"1", 1});
    sut.Insert({"2", 2.5f});
    sut.Insert({"3", String("test")});

    EXPECT_ANY_THROW(sut.At<float>("1"));
    EXPECT_ANY_THROW(sut.At<String>("1"));
    EXPECT_ANY_THROW(sut.At<float>("3"));

    EXPECT_NO_THROW(sut.At<int>("1"));
    EXPECT_NO_THROW(sut.At<float>("2"));
    EXPECT_NO_THROW(sut.At<String>("3"));

    EXPECT_EQ(1, sut.At<int>("1"));
    EXPECT_EQ(2.5f, sut.At<float>("2"));
    EXPECT_EQ("test", sut.At<String>("3"));
}

TEST(Tags, AddIfMissing) {
    Tags sut;
    sut.TypeAddIfNeeded<String>("test", "hello");

    EXPECT_EQ("hello", sut.SafeValue<String>("test"));
}

TEST(Storage, Test) {
    Storage<String> sut;

    // TODO:
}
