#include "gtest/gtest.h"

#include "VectorList.h"
#include "_String.h"

using namespace std;
using namespace PJ;

TEST(List, Remove) {
    VectorList<String> sut;

    sut.Add("test");
    EXPECT_EQ(1, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());
}

TEST(List, RemoveIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    RemoveIf(sut, [](int value) { return value < 4; });
    EXPECT_EQ(2, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(5, *i);

    i++;
    EXPECT_EQ(10, *i);
}

TEST(List, FirstIterator) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIterator(sut, [](int value) { return value == 3; })));
}

TEST(List, FirstIteratorInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIterator(sut, [](int value) { return value == 3; })));
}

TEST(List, First) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto first = First(sut, [](int value) { return value > 1; });
    EXPECT_EQ(std::make_optional<int>(3), first);
}

TEST(List, FirstInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto firstInvalid = First(sut, [](int value) { return value > 11; });
    EXPECT_EQ(std::nullopt, firstInvalid);
}

TEST(List, RemoveFirstIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    RemoveFirstIf(sut, [](int value) { return value < 4; });
    EXPECT_EQ(3, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(3, *i);

    i++;
    EXPECT_EQ(5, *i);
}

TEST(List, Contains) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_FALSE(Contains(sut, -11));
}

TEST(List, AddRange) {
    VectorList<int> sut{ 1 };
    VectorList<int> source{ 3, 5 };

    AddRange(sut, source);
    EXPECT_EQ(3, sut.size());
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(5, sut[2]);
}

TEST(List, IndexOf) {
    VectorList<int> sut{ 1, 3, 5 };

    EXPECT_EQ(0, IndexOf(sut, 1).value());
    EXPECT_EQ(2, IndexOf(sut, 5).value());
    EXPECT_FALSE(IndexOf(sut, 20));
}

TEST(List, Filter) {
    VectorList<int> sut{ 1, 2, 3, 4 };

    auto values = Filter(sut, [](int value) { return value < 3; });
    EXPECT_EQ(2, values.size());

    auto i = values.begin();
    EXPECT_EQ(1, *i);

    i++;
    EXPECT_EQ(2, *i);
}

TEST(List, RemoveAt) {
    VectorList<int> sut{ 1, 2, 3 };
    
    RemoveAt(sut, -1);
    EXPECT_EQ(3, sut.size());
    RemoveAt(sut, 40);
    EXPECT_EQ(3, sut.size());

    RemoveAt(sut, 1);

    VectorList<int> expectedValues{ 1, 3 };
    EXPECT_EQ(expectedValues, sut);
}
