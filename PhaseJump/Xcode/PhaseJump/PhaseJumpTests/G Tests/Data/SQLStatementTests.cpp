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
    EXPECT_EQ("a?", sut.value);
    ASSERT_EQ(1u, sut.parameters.size());
    EXPECT_EQ(SQLValueType::Text, sut.parameters[0].type);
    EXPECT_EQ("b", sut.parameters[0].value);
}

TEST(SQLStatement, TestAppendValue_InjectionIsBound_NotInterpolated) {
    // Single quote in the value must be carried as a parameter, not embedded
    // into the SQL text. Otherwise the value could terminate the quoted literal.
    SQLStatement sut = { "SELECT * FROM users WHERE name=" };
    sut.Append(SQLValue(SQLValueType::Text, "O'Brien'; DROP TABLE users; --"));
    EXPECT_EQ("SELECT * FROM users WHERE name=?", sut.value);
    ASSERT_EQ(1u, sut.parameters.size());
    EXPECT_EQ("O'Brien'; DROP TABLE users; --", sut.parameters[0].value);
}

TEST(SQLStatement, TestAppendIdentifier) {
    SQLStatement sut = { "a" };
    sut.AppendIdentifier("b");
    EXPECT_EQ("a[b]", sut.value);
}
