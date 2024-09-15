#include "gtest/gtest.h"

#include "ResourceModels.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

namespace LoadedResourcesTests {
    class Test : public Base {
    };
}

using namespace LoadedResourcesTests;

TEST(LoadedResources, TestRemove) {
    ResourceModels sut;
    sut.map["type"]["id"] = ResourceModel(MAKE<Base>(), "id", "path", "type");

    EXPECT_EQ("path", sut.map["type"]["id"].info.filePath.string());

    sut.Remove("type", "id");

    EXPECT_EQ("", sut.map["type"]["id"].info.filePath.string());
}
