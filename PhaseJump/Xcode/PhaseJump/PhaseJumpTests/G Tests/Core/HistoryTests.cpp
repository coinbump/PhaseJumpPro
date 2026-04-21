#include "gtest/gtest.h"
#include "History.h"

using namespace PJ;
using namespace std;

TEST(History, Add_BelowLimit) {
    History<int> sut(3);

    EXPECT_TRUE(sut.IsEmpty());

    sut.Add(1);
    sut.Add(2);

    EXPECT_EQ(2, sut.Size());
    EXPECT_EQ(1, sut.Items()[0]);
    EXPECT_EQ(2, sut.Items()[1]);
}

TEST(History, Add_PrunesOldestWhenLimitExceeded) {
    History<int> sut(3);

    sut.Add(1);
    sut.Add(2);
    sut.Add(3);
    sut.Add(4);

    ASSERT_EQ(3, sut.Size());
    EXPECT_EQ(2, sut.Items()[0]);
    EXPECT_EQ(3, sut.Items()[1]);
    EXPECT_EQ(4, sut.Items()[2]);

    sut.Add(5);
    ASSERT_EQ(3, sut.Size());
    EXPECT_EQ(3, sut.Items()[0]);
    EXPECT_EQ(5, sut.Items()[2]);
}

TEST(History, Add_LowerLimitPrunesOnNextAdd) {
    History<int> sut(5);
    sut.Add(1);
    sut.Add(2);
    sut.Add(3);
    sut.Add(4);
    sut.Add(5);
    ASSERT_EQ(5, sut.Size());

    sut.limit = 2;
    sut.Add(6);

    ASSERT_EQ(2, sut.Size());
    EXPECT_EQ(5, sut.Items()[0]);
    EXPECT_EQ(6, sut.Items()[1]);
}

TEST(History, Clear) {
    History<int> sut(3);
    sut.Add(1);
    sut.Add(2);

    sut.Clear();

    EXPECT_EQ(0, sut.Size());
    EXPECT_TRUE(sut.IsEmpty());
}

TEST(History, DefaultLimit) {
    History<int> sut;
    EXPECT_EQ(100, sut.limit);
}

TEST(History, StringType) {
    History<std::string> sut(2);
    sut.Add("a");
    sut.Add("b");
    sut.Add("c");

    ASSERT_EQ(2, sut.Size());
    EXPECT_EQ("b", sut.Items()[0]);
    EXPECT_EQ("c", sut.Items()[1]);
}
