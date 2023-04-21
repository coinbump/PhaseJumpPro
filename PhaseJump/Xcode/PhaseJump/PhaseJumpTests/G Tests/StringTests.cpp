#include "gtest/gtest.h"
#include "_String.h"
#include "VectorList.h"

using namespace PJ;

TEST(String, Suffix_Prefix) {
    String sut("Test");
    EXPECT_TRUE(sut.HasPrefix("T"));
    EXPECT_TRUE(sut.HasPrefix("Te"));
    EXPECT_TRUE(sut.HasPrefix("Test"));
    EXPECT_FALSE(sut.HasPrefix("es"));
    EXPECT_FALSE(sut.HasPrefix("_"));

    EXPECT_TRUE(sut.HasSuffix("t"));
    EXPECT_TRUE(sut.HasSuffix("st"));
    EXPECT_TRUE(sut.HasSuffix("Test"));
    EXPECT_FALSE(sut.HasSuffix("es"));
    EXPECT_FALSE(sut.HasSuffix("_"));
}

TEST(String, TypeValue_Positive) {
    String sut("14");
    EXPECT_EQ(14, sut.UIntValue());
    EXPECT_EQ(14, sut.IntValue());
    EXPECT_EQ(14, sut.Int64Value());
    EXPECT_EQ(14.0f, sut.FloatValue());
    EXPECT_EQ(14.0, sut.DoubleValue());
    EXPECT_EQ(true, sut.BoolValue());
}

TEST(String, TypeValue_Negative) {
    String sut("-14");
    EXPECT_EQ(-14, sut.UIntValue());
    EXPECT_EQ(-14, sut.IntValue());
    EXPECT_EQ(-14, sut.Int64Value());
    EXPECT_EQ(-14.0f, sut.FloatValue());
    EXPECT_EQ(-14.0, sut.DoubleValue());
    EXPECT_EQ(true, sut.BoolValue());
}

TEST(String, TypeValue_Bool_Int) {
    String sutFalse("0");
    String sutTrue("1");
    EXPECT_EQ(false, sutFalse.BoolValue());
    EXPECT_EQ(true, sutTrue.BoolValue());
}

TEST(String, TypeValue_Bool_String) {
    String sutFalse("false");
    String sutTrue("true");

    // stringstream does not parse string literals "true" or "false"
    EXPECT_EQ(false, sutFalse.BoolValue());
    EXPECT_EQ(false, sutTrue.BoolValue());
}

TEST(String, ToUpper) {
    EXPECT_EQ(String("HELLO123"), String("hello123").ToUpper());
}

TEST(String, ToLower) {
    EXPECT_EQ(String("hello123"), String("HELLO123").ToLower());
}

TEST(String, Prefix) {
    EXPECT_EQ(String(""), String("12345").Prefix(0));
    EXPECT_EQ(String("123"), String("12345").Prefix(3));
    EXPECT_EQ(String("12345"), String("12345").Prefix(5));
    EXPECT_EQ(String(""), String("12345").Prefix(6));
}

TEST(String, Suffix) {
    EXPECT_EQ(String(""), String("12345").Suffix(0));
    EXPECT_EQ(String("345"), String("12345").Suffix(3));
    EXPECT_EQ(String("12345"), String("12345").Suffix(5));
    EXPECT_EQ(String(""), String("12345").Suffix(6));
}

TEST(String, ReplacingSuffix) {
    EXPECT_EQ(String("t.h"), String("t.h").ReplacingSuffix("x", "z"));
    EXPECT_EQ(String("t.cs"), String("t.h").ReplacingSuffix(".h", ".cs"));
    EXPECT_EQ(String("t"), String("t.h").ReplacingSuffix(".h", ""));
}

TEST(String, TestComponentsSeparatedBy) {
    auto string = String("test 1 2");
    auto components = string.ComponentsSeparatedBy(' ');
    EXPECT_EQ(3, components.size());
    EXPECT_EQ("test", components[0]);
    EXPECT_EQ("1", components[1]);
    EXPECT_EQ("2", components[2]);
}
