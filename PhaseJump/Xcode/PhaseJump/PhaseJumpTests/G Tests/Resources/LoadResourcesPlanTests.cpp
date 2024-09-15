#include "gtest/gtest.h"

#include "LoadResourcesPlan.h"
#include "Base.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

namespace LoadResourcesPlanTests {
}

using namespace LoadResourcesPlanTests;

TEST(LoadResourcesPlan, TestRemove) {
    LoadResourcesPlan sut;
    sut.infoMap["1"]["id1"] = ResourceInfo();
    sut.infoMap["2"]["id2-a"] = ResourceInfo();
    sut.infoMap["2"]["id2-b"] = ResourceInfo();

    auto allInfos = sut.AllInfos();
    EXPECT_EQ(3, allInfos.size());
}
