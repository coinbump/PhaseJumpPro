#include "gtest/gtest.h"
#include "SharedVectorList.h"

using namespace PJ;
using namespace std;

namespace SharedVectorListTests {
}

using namespace SharedVectorListTests;

TEST(SharedVectorList, Vector)
{
    std::vector<int> source{0, 3, 4};
    SharedVectorList<int> sut(source);
    EXPECT_FALSE(sut.IsShared());
    EXPECT_EQ(3, sut.Modifiable().size());
}

TEST(SharedVectorList, Span)
{
    std::vector<int> source{0, 3, 4};
    SharedVectorList<int> sut{std::span<int>(source)};
    EXPECT_TRUE(sut.IsShared());

    ASSERT_EQ(3, sut.Span().size());
    EXPECT_EQ(0, sut.Span()[0]);
    EXPECT_EQ(3, sut.Span()[1]);
    EXPECT_EQ(4, sut.Span()[2]);

    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(4, sut[2]);
}

TEST(SharedVectorList, SpanModifiable)
{
    std::vector<int> source{0, 3, 4};
    SharedVectorList<int> sut{std::span<int>(source)};
    EXPECT_TRUE(sut.IsShared());

    EXPECT_EQ(3, sut.Modifiable().size());
    EXPECT_FALSE(sut.IsShared());

    EXPECT_EQ(0, sut[0]);
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(4, sut[2]);
}

// Unavailable. At requires C++26
//TEST(SharedVectorList, SpanAt)
//{
//    std::vector<int> source{0, 3, 4};
//    SharedVectorList<int> sut{std::span<int>(source)};
//    EXPECT_TRUE(sut.IsShared());
//
//    EXPECT_NO_THROW(sut.At(0));
//    EXPECT_ANY_THROW(sut.At(100));
//}
