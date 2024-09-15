#include "gtest/gtest.h"
#include "StringUtils.h"
#include "CollectionUtils.h"
#include "VectorList.h"

using namespace PJ;
using namespace std;

namespace StringUtilsTests {
}

using namespace StringUtilsTests;

TEST(StringUtils, U32CharToString)
{
    // FUTURE: update when Unicode support is added
    EXPECT_EQ("a", U32CharToString('a'));
}

TEST(StringUtils, ToU32String)
{
    std::u32string u32 = U"a";

    // FUTURE: update when Unicode support is added
    EXPECT_EQ(u32, ToU32String("a"));
}

TEST(StringUtils, MakeString)
{
    EXPECT_EQ("1", MakeString(1));
    EXPECT_EQ("1.3", MakeString(1.3f));
    EXPECT_EQ("1.3", MakeString(1.3));
}

TEST(StringUtils, CompareNoCase)
{
    EXPECT_TRUE(CompareNoCase("aba", "aBA"));
    EXPECT_FALSE(CompareNoCase("aba", "aB1"));
    EXPECT_FALSE(CompareNoCase("aba", "abaa"));
    EXPECT_FALSE(CompareNoCase("aba", "a"));
}

TEST(StringUtils, Split)
{
    VectorList<String> values;
    Split("abc", 'b', [&](String s) {
                                    values.push_back(s);
                                    });

    ASSERT_EQ(2, values.size());
    EXPECT_EQ("a", values[0]);
    EXPECT_EQ("c", values[1]);
}

TEST(StringUtils, Joined)
{
    VectorList<String> values;
    Split("abc", 'b', [&](String s) {
                                    values.push_back(s);
                                    });

    ASSERT_EQ(2, values.size());
    EXPECT_EQ("a,c", Joined(values, ","));
}

TEST(StringUtils, JoinedInitializer)
{
    EXPECT_EQ("a,c", Joined({"a", "c"}, ","));
}

TEST(StringUtils, Suffix_Prefix) {
    String sut("Test");
    EXPECT_TRUE(HasPrefix(sut, "T"));
    EXPECT_TRUE(HasPrefix(sut, "Te"));
    EXPECT_TRUE(HasPrefix(sut, "Test"));
    EXPECT_FALSE(HasPrefix(sut, "es"));
    EXPECT_FALSE(HasPrefix(sut, "_"));

    EXPECT_TRUE(HasSuffix(sut, "t"));
    EXPECT_TRUE(HasSuffix(sut, "st"));
    EXPECT_TRUE(HasSuffix(sut, "Test"));
    EXPECT_FALSE(HasSuffix(sut, "es"));
    EXPECT_FALSE(HasSuffix(sut, "_"));
}

TEST(StringUtils, Suffix_Repeated) {
    String sut("TestTest");
    EXPECT_TRUE(HasSuffix(sut, "t"));
    EXPECT_TRUE(HasSuffix(sut, "Test"));
}

TEST(StringUtils, Suffix_WithSamePrefix) {
    String sut("TestToo");
    EXPECT_FALSE(HasSuffix(sut, "Test"));
}

TEST(StringUtils, Prefix_BiggerThanString) {
    String sut("t");
    EXPECT_FALSE(HasPrefix(sut, "Test"));
}

TEST(StringUtils, Suffix_BiggerThanString) {
    String sut("t");
    EXPECT_FALSE(HasSuffix(sut, "Test"));
}

TEST(StringUtils, TypeValue_Positive) {
    String sut("14");
    EXPECT_EQ(14, TypeValue<unsigned int>(sut));
    EXPECT_EQ(14, TypeValue<int>(sut));
    EXPECT_EQ(14, TypeValue<int64_t>(sut));
    EXPECT_EQ(14.0f, TypeValue<float>(sut));
    EXPECT_EQ(14.0, TypeValue<double>(sut));
    EXPECT_EQ(true, TypeValue<bool>(sut));
}

TEST(StringUtils, TypeValue_Negative) {
    String sut("-14");
    EXPECT_EQ(-14, TypeValue<unsigned int>(sut));
    EXPECT_EQ(-14, TypeValue<int>(sut));
    EXPECT_EQ(-14, TypeValue<int64_t>(sut));
    EXPECT_EQ(-14.0f, TypeValue<float>(sut));
    EXPECT_EQ(-14.0, TypeValue<double>(sut));
    EXPECT_EQ(true, TypeValue<bool>(sut));
}

TEST(StringUtils, TypeValue_Bool_Int) {
    String sutFalse("0");
    String sutTrue("1");
    EXPECT_EQ(false, TypeValue<bool>(sutFalse));
    EXPECT_EQ(true, TypeValue<bool>(sutTrue));
}

TEST(StringUtils, TypeValue_Bool_String) {
    String sutFalse("false");
    String sutTrue("true");

    // stringstream does not parse string literals "true" or "false"
    EXPECT_EQ(false, TypeValue<bool>(sutFalse));
    EXPECT_EQ(false, TypeValue<bool>(sutTrue));
}

TEST(StringUtils, ToUpper) {
    EXPECT_EQ(String("HELLO123"), ToUpper("hello123"));
}

TEST(StringUtils, ToLower) {
    EXPECT_EQ(String("hello123"), ToLower("HELLO123"));
}

TEST(StringUtils, Prefix) {
    EXPECT_EQ(String(""), Prefix("12345", 0));
    EXPECT_EQ(String("123"), Prefix("12345", 3));
    EXPECT_EQ(String("12345"), Prefix("12345", 5));
    EXPECT_EQ(String(""), Prefix("12345", 6));
}

TEST(StringUtils, Suffix) {
    EXPECT_EQ(String(""), Suffix("12345", 0));
    EXPECT_EQ(String("345"), Suffix("12345", 3));
    EXPECT_EQ(String("12345"), Suffix("12345", 5));
    EXPECT_EQ(String(""), Suffix("12345", 6));
}

TEST(StringUtils, ReplacingSuffix) {
    EXPECT_EQ(String("t.h"), ReplacingSuffix("t.h", "x", "z"));
    EXPECT_EQ(String("t.cs"), ReplacingSuffix("t.h", ".h", ".cs"));
    EXPECT_EQ(String("t"), ReplacingSuffix("t.h", ".h", ""));
}

TEST(StringUtils, TestComponentsSeparatedBy) {
    auto string = String("test 1 2");
    auto components = ComponentsSeparatedBy(string, ' ');
    EXPECT_EQ(3, components.size());
    EXPECT_EQ("test", components[0]);
    EXPECT_EQ("1", components[1]);
    EXPECT_EQ("2", components[2]);
}

TEST(StringUtils, TestFilter) {
    auto sut = String("test12");
    auto sut2 = Filter(sut, [] (char c) { return c == '1'; });
    EXPECT_EQ("1", sut2);
}

TEST(StringUtils, TestRemoveIf) {
    auto sut = String("test12");
    RemoveIf(sut, [] (char c) { return c == '1'; });
    EXPECT_EQ("test2", sut);
}

TEST(StringUtils, IsNumbers) {
    EXPECT_FALSE(IsNumbers("123.0"));
    EXPECT_FALSE(IsNumbers("123a0"));
    EXPECT_FALSE(IsNumbers("-123a0"));
    EXPECT_TRUE(IsNumbers("1230"));
}

TEST(StringUtils, IsNumericInt) {
    EXPECT_FALSE(IsNumericInt("123.0"));
    EXPECT_FALSE(IsNumericInt("123a0"));
    EXPECT_TRUE(IsNumericInt("1230"));
    EXPECT_TRUE(IsNumericInt("-1230"));
}

TEST(StringUtils, IsNumericReal) {
    // FUTURE: make this locale independent for non-US countries
    EXPECT_TRUE(IsNumericReal("3123.0"));
    EXPECT_TRUE(IsNumericReal("-3123.0"));
    EXPECT_FALSE(IsNumericReal("123a0"));
    EXPECT_TRUE(IsNumericReal("1230"));
}

TEST(StringUtils, MakeStringFolded) {
    EXPECT_EQ("135", MakeString(1, 3, 5));
    EXPECT_EQ("1,5", MakeString(1, ",", 5));
}
