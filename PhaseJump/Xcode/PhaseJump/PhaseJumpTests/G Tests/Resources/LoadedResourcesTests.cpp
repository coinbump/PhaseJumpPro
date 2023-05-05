#include "gtest/gtest.h"

#include "LoadedResources.h"
#include "_String.h"

using namespace std;
using namespace PJ;

namespace LoadedResourcesTests {
    class Test : public Base {
    };
}

using namespace LoadedResourcesTests;

TEST(LoadedResources, TestRemove) {
    LoadedResources sut;
    sut.map["type"]["id"] = LoadedResource("path", "type", "id", MAKE<Base>());

    EXPECT_EQ("path", sut.map["type"]["id"].filePath.string());

    sut.Remove("type", "id");

    EXPECT_EQ("", sut.map["type"]["id"].filePath.string());
}
