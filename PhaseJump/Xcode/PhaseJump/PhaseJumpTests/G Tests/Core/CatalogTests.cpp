#include "gtest/gtest.h"
#include "Catalog.h"
#include "StringUtils.h"
#include "Utils.h"

using namespace PJ;
using namespace std;

namespace CatalogTests {
}

using namespace CatalogTests;

TEST(Catalog, Test) {
    Catalog<int> sut;
    EXPECT_EQ(std::nullopt, sut.Find("type", "id"));
    
    sut.Add("type", "id", 10);
    ASSERT_NE(std::nullopt, sut.Find("type", "id"));
    EXPECT_EQ(10, *sut.Find("type", "id"));
    
    sut.Remove("type", "id");
    EXPECT_EQ(std::nullopt, sut.Find("type", "id"));
}
