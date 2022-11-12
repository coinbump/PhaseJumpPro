#include "gtest/gtest.h"
#include "Core/Tags.h"

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

    // Template for casting
//    try
//    {
//    }
//    catch (const std::bad_any_cast& e) <- or catch(...)
//    {
//    }
}

TEST(Tags, Values) {
    Tags sut;
    sut.Add("1", 1);
    sut.Add("2", 2.5f);
    sut.Add("3", std::string("test"));

    EXPECT_TRUE(sut.ContainsTypedValue<int>("1"));
    EXPECT_TRUE(sut.ContainsTypedValue<float>("2"));
    EXPECT_TRUE(sut.ContainsTypedValue<string>("3"));

    EXPECT_EQ(1, sut.SafeValue<int>("1"));
    EXPECT_EQ(2.5f, sut.SafeValue<float>("2"));
    EXPECT_EQ("test", sut.SafeValue<string>("3"));

    EXPECT_EQ(0, sut.SafeValue<int>("X"));
    EXPECT_EQ(0.0f, sut.SafeValue<float>("X"));
    EXPECT_EQ("", sut.SafeValue<string>("X"));
}

TEST(Tags, Value) {
    Tags sut;
    sut.Add("1", 1);
    sut.Add("2", 2.5f);
    sut.Add("3", std::string("test"));

    EXPECT_EQ(1, sut.Value<int>("1"));
    EXPECT_EQ(2.5f, sut.Value<float>("2"));
    EXPECT_EQ("test", sut.Value<string>("3"));

    EXPECT_EQ(std::nullopt, sut.Value<int>("X"));
    EXPECT_EQ(std::nullopt, sut.Value<float>("X"));
    EXPECT_EQ(std::nullopt, sut.Value<string>("X"));
}

TEST(Tags, SharedPtr) {
    Tags sut;
    sut.Add("1", make_shared<TestClass>());
    EXPECT_NE(nullptr, sut.Value<shared_ptr<TestClass>>("1"));
}
