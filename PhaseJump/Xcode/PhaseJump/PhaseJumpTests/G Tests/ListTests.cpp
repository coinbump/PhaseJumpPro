#include "gtest/gtest.h"

#include "VectorList.h"
#include "_String.h"

using namespace std;
using namespace PJ;

TEST(List, Remove) {
    VectorList<String> sut;

    sut.Add("test");
    EXPECT_EQ(1, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());
}

TEST(List, RemoveIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    sut.RemoveIf([](int value) { return value < 4; });
    EXPECT_EQ(2, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(5, *i);

    i++;
    EXPECT_EQ(10, *i);
}

TEST(List, RemoveFirstIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    sut.RemoveFirstIf([](int value) { return value < 4; });
    EXPECT_EQ(3, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(3, *i);

    i++;
    EXPECT_EQ(5, *i);
}

TEST(List, Contains) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_TRUE(sut.Contains(1));
    EXPECT_FALSE(sut.Contains(-11));
}
