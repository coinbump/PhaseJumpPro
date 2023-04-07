#include "gtest/gtest.h"

#include "SQLStatement.h"

using namespace std;
using namespace PJ;

TEST(SQLStatement, TestAppendString) {
    SQLStatement sut = { "a" };
    sut.AppendString("b");
    EXPECT_EQ("ab", sut.value);
}

TEST(SQLStatement, TestAppendValue) {
    SQLStatement sut = { "a" };
    sut.Append(SQLValue(SQLValueType::Text, "b"));
    EXPECT_EQ("a'b'", sut.value);
}

TEST(SQLStatement, TestAppendIdentifier) {
    SQLStatement sut = { "a" };
    sut.AppendIdentifier("b");
    EXPECT_EQ("a[b]", sut.value);
}
