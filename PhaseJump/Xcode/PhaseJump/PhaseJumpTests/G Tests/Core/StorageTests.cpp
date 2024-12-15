#include "gtest/gtest.h"
#include "Tags.h"
#include "StringUtils.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace StorageTests {
}

using namespace StorageTests;

TEST(Storage, Test) {
    Storage<> sut;
    EXPECT_EQ(10.0f, sut.Value<float>("group", "test", 10));
    sut.Set("group", "test", 15.0f);
    EXPECT_EQ(15.0f, sut.Value<float>("group", "test", 10));

    EXPECT_ANY_THROW(sut.At<int>("group", "test"));
    EXPECT_NO_THROW(sut.At<float>("group", "test"));
    
    EXPECT_EQ(15.0f, sut.At<float>("group", "test"));
}
