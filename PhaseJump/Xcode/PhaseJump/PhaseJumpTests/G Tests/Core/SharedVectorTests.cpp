#include "gtest/gtest.h"
#include "SharedVector.h"

using namespace PJ;
using namespace std;

namespace SharedVectorTests {
}

using namespace SharedVectorTests;

TEST(SharedVector, Vector)
{
    std::vector<int> source{0, 3, 4};
    SharedVector<int> sut(source);
    EXPECT_TRUE(sut.IsModifiable());
    EXPECT_FALSE(sut.IsShared());
    EXPECT_EQ(3, sut.Modifiable().size());
}

TEST(SharedVector, Span)
{
    std::vector<int> source{0, 3, 4};
    SharedVector<int> sut{std::span<int>(source)};
    EXPECT_FALSE(sut.IsModifiable());
    EXPECT_TRUE(sut.IsShared());

    ASSERT_EQ(3, sut.Value().size());
    EXPECT_EQ(0, sut.Value()[0]);
    EXPECT_EQ(3, sut.Value()[1]);
    EXPECT_EQ(4, sut.Value()[2]);

    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(4, sut[2]);
}

TEST(SharedVector, SpanModifiable)
{
    std::vector<int> source{0, 3, 4};
    SharedVector<int> sut{std::span<int>(source)};
    EXPECT_FALSE(sut.IsModifiable());
    EXPECT_TRUE(sut.IsShared());

    EXPECT_EQ(3, sut.Modifiable().size());
    EXPECT_FALSE(sut.IsShared());
    EXPECT_TRUE(sut.IsModifiable());

    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(4, sut[2]);
}
