#include "gtest/gtest.h"

#include "SQLTool.h"

using namespace std;
using namespace PJ;

TEST(SQLTool, IsReservedKeyword) {
    SQLTool sut;

    EXPECT_TRUE(sut.IsReservedKeyword("where"));
    EXPECT_TRUE(sut.IsReservedKeyword("WHERE"));
    EXPECT_TRUE(sut.IsReservedKeyword("cascade"));
    EXPECT_TRUE(sut.IsReservedKeyword("CASCADE"));
    EXPECT_TRUE(sut.IsReservedKeyword("VACUUM"));

    EXPECT_FALSE(sut.IsReservedKeyword("my_table"));
}
