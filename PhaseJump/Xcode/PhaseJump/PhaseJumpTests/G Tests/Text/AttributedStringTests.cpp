#include "gtest/gtest.h"
#include "AttributedString.h"
#include "Font.h"

using namespace PJ;
using namespace std;

namespace AttributedStringTests {
    class TestAttributedString : public AttributedString {
    public:
        TestAttributedString(Config config = {}) : AttributedString(config) {
        }

        void AddTextPart(String value) {
            parts.push_back({.text = value});
        }

        auto PartAt(size_t index) {
            return parts[index];
        }
        
        void TestApplyAttribute(TextPart part, int start, int delta) {
            ApplyAttribute(part, start, delta);
        }
        
        void TestInsertPart(TextPart part, int pos) {
            InsertPart(part, pos);
        }
    };
}

using namespace AttributedStringTests;

TEST(AttributedString, PlainTextSingle)
{
    TestAttributedString sut;
    EXPECT_EQ("", sut.PlainText());
    
    sut.SetPlainText("test");
    EXPECT_EQ("test", sut.PlainText());
    
    EXPECT_EQ(1, sut.PartCount());
}

TEST(AttributedString, PlainTextMultiple)
{
    TestAttributedString sut{{.text = "te"}};
    EXPECT_EQ("te", sut.PlainText());
    
    sut.AddTextPart("st");
    EXPECT_EQ("test", sut.PlainText());
    
    EXPECT_EQ(2, sut.PartCount());
}

TEST(AttributedString, InsertPartBefore)
{
    TestAttributedString sut({.text = "t"});
    
    sut.TestInsertPart({.text = "_"}, 0);
    ASSERT_EQ(2, sut.PartCount());
    
    EXPECT_EQ("_", sut.PartAt(0).text);
    EXPECT_EQ("t", sut.PartAt(1).text);
}

TEST(AttributedString, InsertPartAfter)
{
    TestAttributedString sut({.text = "t"});
    
    sut.TestInsertPart({.text = "_"}, 1);
    ASSERT_EQ(2, sut.PartCount());
    
    EXPECT_EQ("t", sut.PartAt(0).text);
    EXPECT_EQ("_", sut.PartAt(1).text);
}

TEST(AttributedString, InsertPartSplit)
{
    TestAttributedString sut({.text = "te"});
    
    sut.TestInsertPart({.text = "_"}, 1);
    ASSERT_EQ(3, sut.PartCount());
    
    EXPECT_EQ("t", sut.PartAt(0).text);
    EXPECT_EQ("_", sut.PartAt(1).text);
    EXPECT_EQ("e", sut.PartAt(2).text);
}

TEST(AttributedString, ApplyAttributeEndIsAtEndOfString)
{
    TestAttributedString sut({.text = "te"});
    
    sut.TestApplyAttribute({.type = TextPartType::AttributeColor, .color = Color::red}, 1, 1);
    ASSERT_EQ(3, sut.PartCount());
    
    EXPECT_EQ("t", sut.PartAt(0).text);
    EXPECT_EQ("", sut.PartAt(1).text);
    EXPECT_EQ(Color::red, sut.PartAt(1).color);
    EXPECT_EQ(1, sut.PartAt(1).attributeLength);
    EXPECT_EQ(TextPartType::AttributeColor, sut.PartAt(1).type);
    EXPECT_EQ("e", sut.PartAt(2).text);
}

TEST(AttributedString, ApplyAttributeEndIsInsideString)
{
    TestAttributedString sut({.text = "tes"});
    
    sut.TestApplyAttribute({.type = TextPartType::AttributeColor, .color = Color::red}, 1, 1);
    ASSERT_EQ(3, sut.PartCount());
    
    EXPECT_EQ("t", sut.PartAt(0).text);
    EXPECT_EQ("", sut.PartAt(1).text);
    EXPECT_EQ(Color::red, sut.PartAt(1).color);
    EXPECT_EQ(TextPartType::AttributeColor, sut.PartAt(1).type);
    EXPECT_EQ(1, sut.PartAt(1).attributeLength);
    EXPECT_EQ("es", sut.PartAt(2).text);
}

TEST(AttributedString, Attributes)
{
    TestAttributedString sut({.text = "tes"});
    
    sut.ApplyColor(Color::red, 1, 1);
    
    ASSERT_EQ(0, sut.Attributes(0).size());
    
    auto attributes = sut.Attributes(1);
    ASSERT_EQ(1, attributes.size());
    
    EXPECT_EQ(TextPartType::AttributeColor, attributes[0].type);
    EXPECT_EQ(Color::red, attributes[0].color);
    
    ASSERT_EQ(0, sut.Attributes(2).size());
}

TEST(AttributedString, GetColor)
{
    TestAttributedString sut({.text = "tes"});
    
    sut.ApplyColor(Color::green, 1, 1);
    sut.ApplyColor(Color::red, 0, 1);
    sut.ApplyColor(Color::blue, 2, 1);
    
    EXPECT_EQ(Color::red, sut.GetColor(0));
    EXPECT_EQ(Color::green, sut.GetColor(1));
    EXPECT_EQ(Color::blue, sut.GetColor(2));
}
