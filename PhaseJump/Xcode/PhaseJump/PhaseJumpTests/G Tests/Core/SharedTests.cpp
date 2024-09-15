#include "gtest/gtest.h"
#include "Shared.h"

using namespace PJ;
using namespace std;

namespace SharedTests {
}

using namespace SharedTests;

TEST(Shared, Vector)
{
    std::vector<int> source{0, 3, 4};
    Shared<std::vector<int>> sut(source);
    EXPECT_TRUE(sut.IsModifiable());
    EXPECT_FALSE(sut.IsShared());
    EXPECT_EQ(3, sut.Modifiable().size());
}

TEST(Shared, Ptr)
{
    std::vector<int> source{0, 3, 4};
    Shared<std::vector<int>> sut{&source};
    EXPECT_FALSE(sut.IsModifiable());
    EXPECT_TRUE(sut.IsShared());

    auto value = sut.Value();
    ASSERT_NE(nullptr, value);
    ASSERT_EQ(3, value->size());
    EXPECT_EQ(0, (*value)[0]);
    EXPECT_EQ(3, (*value)[1]);
    EXPECT_EQ(4, (*value)[2]);
}

TEST(Shared, PtrModifiable)
{
    std::vector<int> source{0, 3, 4};
    Shared<std::vector<int>> sut{&source};
    EXPECT_FALSE(sut.IsModifiable());
    EXPECT_TRUE(sut.IsShared());

    EXPECT_EQ(3, sut.Modifiable().size());
    EXPECT_FALSE(sut.IsShared());
    EXPECT_TRUE(sut.IsModifiable());

    auto value = sut.Modifiable();
    ASSERT_EQ(3, value.size());
    EXPECT_EQ(0, value[0]);
    EXPECT_EQ(3, value[1]);
    EXPECT_EQ(4, value[2]);
}
