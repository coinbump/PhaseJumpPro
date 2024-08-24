#include "gtest/gtest.h"

#include "VectorList.h"
#include "_String.h"

using namespace std;
using namespace PJ;

namespace CollectionUtilsTests {
    struct StringNode {
        String value;

        StringNode(String value) : value(value) {
        }

        VectorList<SP<StringNode>> children;

        VectorList<SP<StringNode>> const& Children() const { return children; }

        SP<StringNode> push_back(String value) {
            SP<StringNode> result = MAKE<StringNode>(value);
            children.push_back(result);
            return result;
        }
    };
}

using namespace CollectionUtilsTests;

TEST(CollectionUtils, Remove) {
    VectorList<String> sut;

    sut.push_back("test");
    EXPECT_EQ(1, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());

    Remove(sut, "test");
    EXPECT_EQ(0, sut.size());
}

TEST(CollectionUtils, RemoveIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    RemoveIf(sut, [](int value) { return value < 4; });
    EXPECT_EQ(2, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(5, *i);

    i++;
    EXPECT_EQ(10, *i);
}

TEST(CollectionUtils, FirstIterator) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIterator(sut, [](int value) { return value == 3; })));
}

TEST(CollectionUtils, FirstIteratorInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIterator(sut, [](int value) { return value == 3; })));
}

TEST(CollectionUtils, First) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto first = First(sut, [](int value) { return value > 1; });
    EXPECT_EQ(std::make_optional<int>(3), first);
}

TEST(CollectionUtils, FirstInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto firstInvalid = First(sut, [](int value) { return value > 11; });
    EXPECT_EQ(std::nullopt, firstInvalid);
}

TEST(CollectionUtils, RemoveFirstIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    RemoveFirstIf(sut, [](int value) { return value < 4; });
    EXPECT_EQ(3, sut.size());

    auto i = sut.begin();
    EXPECT_EQ(3, *i);

    i++;
    EXPECT_EQ(5, *i);
}

TEST(CollectionUtils, Contains) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_TRUE(Contains(sut, 1));
    EXPECT_FALSE(Contains(sut, -11));
}

TEST(CollectionUtils, AddRange) {
    VectorList<int> sut{ 1 };
    VectorList<int> source{ 3, 5 };

    AddRange(sut, source);
    EXPECT_EQ(3, sut.size());
    EXPECT_EQ(3, sut[1]);
    EXPECT_EQ(5, sut[2]);
}

TEST(CollectionUtils, IndexOf) {
    VectorList<int> sut{ 1, 3, 5 };

    EXPECT_EQ(0, IndexOf(sut, 1).value());
    EXPECT_EQ(2, IndexOf(sut, 5).value());
    EXPECT_FALSE(IndexOf(sut, 20));
}

TEST(CollectionUtils, Filter) {
    VectorList<int> sut{ 1, 2, 3, 4 };

    auto values = Filter(sut, [](int value) { return value < 3; });
    EXPECT_EQ(2, values.size());

    auto i = values.begin();
    EXPECT_EQ(1, *i);

    i++;
    EXPECT_EQ(2, *i);
}

TEST(CollectionUtils, RemoveAt) {
    VectorList<int> sut{ 1, 2, 3 };
    
    RemoveAt(sut, -1);
    EXPECT_EQ(3, sut.size());
    RemoveAt(sut, 40);
    EXPECT_EQ(3, sut.size());

    RemoveAt(sut, 1);

    VectorList<int> expectedValues{ 1, 3 };
    EXPECT_EQ(expectedValues, sut);
}

TEST(CollectionUtils, CollectBreadthFirstTree) {
    auto root = MAKE<StringNode>("root");
    auto a = root->push_back("a");
    auto b = root->push_back("b");
    auto c = root->push_back("c");
    auto d = a->push_back("d");
    auto d1 = d->push_back("d1");
    auto d2 = d->push_back("d2");
    auto e = c->push_back("e");

    VectorList<SP<StringNode>> nodes;
    CollectBreadthFirstTree(root, nodes);

    ostringstream os;
    for (auto& node : nodes) {
        os << node->value;
    }

    EXPECT_EQ("rootabcdd1d2e", os.str());
}

TEST(CollectionUtils, IsValidIndex) {
    vector<int> sut;

    sut.resize(1);
    EXPECT_FALSE(IsValidIndex(sut, -1));
    EXPECT_TRUE(IsValidIndex(sut, 0));
    EXPECT_FALSE(IsValidIndex(sut, 1));
}
