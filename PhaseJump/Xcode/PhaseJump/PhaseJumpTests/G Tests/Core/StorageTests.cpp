#include "gtest/gtest.h"
#include "Tags.h"
#include "StringUtils.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace StorageTests {
}

using namespace StorageTests;

TEST(Storage, TestFloat) {
    Storage<> sut;
    EXPECT_EQ(10.0f, sut.SafeValue<float>("group", "test", 10));
    sut.Set("group", "test", 15.0f);
    EXPECT_EQ(15.0f, sut.SafeValue<float>("group", "test", 10));

    EXPECT_ANY_THROW(sut.At<int>("group", "test"));
    EXPECT_NO_THROW(sut.At<float>("group", "test"));
    
    EXPECT_EQ(15.0f, sut.At<float>("group", "test"));
}

TEST(Storage, TestInt) {
    Storage<> sut;
    sut.Set("key", "test", 10);
    
    EXPECT_EQ(10, sut.SafeValue<int>("key", "test"));
    EXPECT_EQ(std::nullopt, sut.Value<int>("key", ""));
    ASSERT_NE(std::nullopt, sut.Value<int>("key", "test"));
    EXPECT_EQ(10, *sut.Value<int>("key", "test"));
    
    EXPECT_ANY_THROW(sut.At<int>("key", ""));
    EXPECT_NO_THROW(sut.At<int>("key", "test"));
}
