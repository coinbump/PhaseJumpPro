#include "gtest/gtest.h"

#include <memory>
#include "_Map.h"

using namespace std;
using namespace PJ;

TEST(Map, ContainsKey) {
    Map<std::string, int> sut;

    sut["1"] = 1;
    sut["2"] = 2;

    EXPECT_TRUE(sut.ContainsKey("1"));
    EXPECT_FALSE(sut.ContainsKey("apple"));
}

TEST(Map, ContainsKeyWhere) {
    Map<std::string, int> sut;

    sut["1"] = 1;
    sut["2"] = 100;

    using Pair = pair<std::string, int>;
    EXPECT_TRUE(sut.ContainsWhere([](int value) -> bool { return value == 1; }));
    EXPECT_TRUE(sut.ContainsWhere([](int value) -> bool { return value == 100; }));
    EXPECT_FALSE(sut.ContainsWhere([](int value) -> bool { return value == 90; }));
}
