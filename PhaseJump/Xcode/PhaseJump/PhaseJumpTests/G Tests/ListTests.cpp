#include "gtest/gtest.h"

#include "List.h"
#include "_String.h"

using namespace std;
using namespace PJ;

TEST(List, Remove) {
    List<String> sut;

    sut.Add("test");
    EXPECT_EQ(1, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());

    sut.Remove("test");
    EXPECT_EQ(0, sut.size());
}

TEST(List, RemoveIf) {
    List<int> sut{ 1, 3, 5, 10 };

    sut.RemoveIf([](int value) { return value < 4; });
    EXPECT_EQ(2, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(5, *i);

    i++;
    EXPECT_EQ(10, *i);
}

TEST(List, RemoveFirstIf) {
    List<int> sut{ 1, 3, 5, 10 };

    sut.RemoveFirstIf([](int value) { return value < 4; });
    EXPECT_EQ(3, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(3, *i);

    i++;
    EXPECT_EQ(5, *i);
}

