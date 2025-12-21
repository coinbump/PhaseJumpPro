#include "gtest/gtest.h"

#include <memory>
#include "OrderedMap.h"
#include "CollectionUtils.h"

using namespace std;
using namespace PJ;

TEST(OrderedMap, ContainsKey) {
    OrderedMap<std::string, int> sut;

    sut["1"] = 1;
    sut["2"] = 2;

    auto value = ContainsKey(sut, "1");
    EXPECT_TRUE(value);

    value = ContainsKey(sut, "apple");
    EXPECT_FALSE(value);
}

TEST(OrderedMap, ContainsKeyWhere) {
    using Map = OrderedMap<std::string, int>;
    Map sut;

    sut["1"] = 1;
    sut["2"] = 100;

    using Pair = pair<std::string, int>;

    auto value = ContainsIf(sut, [](auto& pair) -> bool { return pair.second == 1; });
    EXPECT_TRUE(value);

    value = ContainsIf(sut, [](auto& pair) -> bool { return pair.second == 100; });
    EXPECT_TRUE(value);

    value = ContainsIf(sut, [](auto& pair) -> bool { return pair.second == 90; });
    EXPECT_FALSE(value);
}
