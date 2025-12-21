#include "gtest/gtest.h"

#include "VectorList.h"
#include "StringUtils.h"
#include "UnorderedSet.h"

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

TEST(CollectionUtils, RemoveAll) {
    VectorList<int> sut{ 1, 3, 5, 10 };
    RemoveAll(sut);

    EXPECT_EQ(0, sut.size());
}

TEST(CollectionUtils, Count) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(4, Count(sut));
}

TEST(CollectionUtils, IsEmpty) {
    VectorList<int> sut{ 1, 3, 5, 10 };
    EXPECT_FALSE(IsEmpty(sut));
    RemoveAll(sut);
    EXPECT_TRUE(IsEmpty(sut));
}

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

TEST(CollectionUtils, Compact) {
    int value = 10;
    int value2 = 10;
    VectorList<int*> sut{ &value, nullptr, nullptr, &value2 };
    Compact(sut);
    VectorList<int*> expectedValues{ &value, &value2 };
    EXPECT_EQ(expectedValues, sut);
}

TEST(CollectionUtils, CompactMap) {
    int value = 10;
    int value2 = 10;
    VectorList<int*> sut{ &value, nullptr, nullptr, &value2 };
    auto values = CompactMap<int*>(sut, [](auto& value) { return value; });
    VectorList<int*> expectedValues{ &value, &value2 };
    EXPECT_EQ(expectedValues, values);
}

TEST(CollectionUtils, FirstIterator) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIteratorIf(sut, [](int value) { return value == 3; })));
}

TEST(CollectionUtils, FirstIteratorInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_EQ(3, *(FirstIteratorIf(sut, [](int value) { return value == 3; })));
}

TEST(CollectionUtils, FirstIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto first = FirstIf(sut, [](int value) { return value > 1; });
    EXPECT_EQ(std::make_optional<int>(3), first);
}

TEST(CollectionUtils, SafeFirstWithValue) {
    VectorList<int> sut{ 1 };

    auto first = SafeFirst(sut, 6);
    EXPECT_EQ(1, first);
}

TEST(CollectionUtils, SafeFirstWithNoValue) {
    VectorList<int> sut{};

    auto first = SafeFirst(sut, 6);
    EXPECT_EQ(6, first);
}

TEST(CollectionUtils, FirstInvalid) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    auto firstInvalid = FirstIf(sut, [](int value) { return value > 11; });
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

TEST(CollectionUtils, ContainsIf) {
    VectorList<int> sut{ 1, 3, 5, 10 };

    EXPECT_TRUE(ContainsIf(sut, [](auto& e) { return e == 10; }));
    EXPECT_FALSE(ContainsIf(sut, [](auto& e) { return e == -11; }));
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

TEST(CollectionUtils, IndexOfIf) {
    VectorList<int> sut{ 1, 3, 5 };

    EXPECT_EQ(0, IndexOfIf(sut, [](auto& item) { return item > 0; }).value());
    EXPECT_EQ(2, IndexOfIf(sut, [](auto& item) { return item > 3; }).value());
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


TEST(CollectionUtils, Map) {
    VectorList<int> sut{ 1, 2, 3 };

    auto values = Map<String>(sut, [](int value) { return MakeString(value); });
    EXPECT_EQ(3, values.size());

    VectorList<String> expectedValues{ "1", "2", "3" };
    EXPECT_EQ(expectedValues, values);
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

    VectorList<StringNode*> nodes;
    CollectBreadthFirstTree(root.get(), nodes);

    ostringstream os;
    for (auto& node : nodes) {
        os << node->value;
    }

    EXPECT_EQ("rootabcdd1d2e", os.str());
}

TEST(CollectionUtils, CollectBreadthFirstTreeIf) {
    auto root = MAKE<StringNode>("root");
    auto a = root->push_back("r1");
    auto b = root->push_back("r2");
    auto c = root->push_back("c");
    auto d = a->push_back("r3");
    auto d1 = d->push_back("d1");
    auto d2 = d->push_back("d2");
    auto e = b->push_back("r4");

    VectorList<StringNode*> nodes;
    CollectBreadthFirstTreeIf(root.get(), nodes, [](StringNode* node) {
        return StartsWith(node->value, "r");
    });

    ostringstream os;
    for (auto& node : nodes) {
        os << node->value;
    }

    EXPECT_EQ("rootr1r2r3r4", os.str());
}

TEST(CollectionUtils, IsValidIndex) {
    vector<int> sut;

    sut.resize(1);
    EXPECT_FALSE(IsValidIndex(sut, -1));
    EXPECT_TRUE(IsValidIndex(sut, 0));
    EXPECT_FALSE(IsValidIndex(sut, 1));
}

TEST(CollectionUtils, Append) {
    vector<int> sut;

    EXPECT_FALSE(Contains(sut, 1));
    Append(sut, 1);
    EXPECT_TRUE(Contains(sut, 1));
}

TEST(CollectionUtils, Add) {
    vector<int> sut;

    EXPECT_FALSE(Contains(sut, 1));
    Add(sut, 1);
    EXPECT_TRUE(Contains(sut, 1));
}
