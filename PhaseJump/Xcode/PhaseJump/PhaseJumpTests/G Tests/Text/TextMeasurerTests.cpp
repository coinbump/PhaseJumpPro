#include "gtest/gtest.h"
#include "TextMeasurer.h"
#include "Font.h"

using namespace PJ;
using namespace std;

namespace TextMeasurerTests {
    Font MakeFont() {
        Font font("", 12);
        Font::Glyph ag;
        ag.offset = Vector2Int(-1, 0);
        ag.advanceX = 5;
        ag.size.x = 6;

        Font::Glyph bg;
        bg.offset = Vector2Int(0, 0);
        bg.advanceX = 9;
        bg.size.x = 9;

        font.glyphs['a'] = ag;
        font.glyphs['b'] = bg;
        font.metrics.ascent = 4;
        font.metrics.descent = 2;
        font.metrics.leading = 10;

        return font;
    }
    
    Font MakeFontV2() {
        Font font("", 12);
        Font::Glyph ag;
        ag.offset = Vector2Int(-1, 0);
        ag.advanceX = 5;
        ag.size.x = 12;

        Font::Glyph bg;
        bg.offset = Vector2Int(0, 0);
        bg.advanceX = 9;
        bg.size.x = 9;

        font.glyphs['a'] = ag;
        font.glyphs['b'] = bg;
        font.metrics.ascent = 4;
        font.metrics.descent = 2;
        font.metrics.leading = 10;

        return font;
    }
}

using namespace TextMeasurerTests;

TEST(TextMeasurer, NoRoomToWrap)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", {}, LineClip::None).lines;

    EXPECT_EQ(0, values.size());
}

TEST(TextMeasurer, SingleLineNoClip)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, 0), LineClip::None).lines;

    ASSERT_EQ(1, values.size());
    EXPECT_EQ("aba", values[0].text);
    EXPECT_EQ(0, values[0].y);
    EXPECT_EQ(Vector2(19, sut.font.Height()), values[0].size);
}

TEST(TextMeasurer, SingleLineUseWidthForFinalCharNotAdvance)
{
    auto font = MakeFontV2();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, 0), LineClip::None).lines;

    ASSERT_EQ(1, values.size());
    EXPECT_EQ("aba", values[0].text);
    EXPECT_EQ(0, values[0].y);
    
    // The final character on a line doesn't advance to another character, so use its width instead of advance to determine line size
    EXPECT_EQ(Vector2(25, sut.font.Height()), values[0].size);
}

TEST(TextMeasurer, SingleLinePartialClipWithClip)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, font.Height() - 1), LineClip::Partial).lines;

    ASSERT_EQ(0, values.size());
}

TEST(TextMeasurer, SingleLinePartialClipNoClip)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, font.Height()), LineClip::Partial).lines;

    ASSERT_EQ(1, values.size());
    EXPECT_EQ("aba", values[0].text);
    EXPECT_EQ(0, values[0].y);
    EXPECT_EQ(Vector2(19, sut.font.Height()), values[0].size);
}

TEST(TextMeasurer, SingleLineHiddenClipWithClip)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, -1), LineClip::Hidden).lines;

    ASSERT_EQ(0, values.size());
}

TEST(TextMeasurer, SingleLineHiddenClipNoClip)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(1000, font.Height() - 3), LineClip::Hidden).lines;

    ASSERT_EQ(1, values.size());
    EXPECT_EQ("aba", values[0].text);
    EXPECT_EQ(0, values[0].y);
    EXPECT_EQ(Vector2(19, sut.font.Height()), values[0].size);
}

TEST(TextMeasurer, MultiLineWrap)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(14, font.Height()), LineClip::None).lines;

    ASSERT_EQ(2, values.size());
    EXPECT_EQ("ab", values[0].text);
    EXPECT_EQ("a", values[1].text);
    EXPECT_EQ(0, values[0].sourceIndex);
    EXPECT_EQ(2, values[1].sourceIndex);
    EXPECT_EQ(0, values[0].y);
    EXPECT_EQ(font.metrics.leading, values[1].y);
    EXPECT_EQ(Vector2(14, sut.font.Height()), values[0].size);
    EXPECT_EQ(Vector2(5, sut.font.Height()), values[1].size);
}

TEST(TextMeasurer, MultiLineWrapWithNewline)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("a\nb\na", Vector2(1000, font.Height()), LineClip::None).lines;

    ASSERT_EQ(3, values.size());
    EXPECT_EQ("a", values[0].text);
    EXPECT_EQ("b", values[1].text);
    EXPECT_EQ("a", values[2].text);
    EXPECT_EQ(0, values[0].sourceIndex);
    EXPECT_EQ(2, values[1].sourceIndex);
    EXPECT_EQ(4, values[2].sourceIndex);
    EXPECT_EQ(0, values[0].y);
}

TEST(TextMeasurer, MultiLineWrapWithMetricsFunc)
{
    auto font = MakeFont();
    TextMeasurer sut(font);
    auto values = sut.Measure("aba", Vector2(14, font.Height()), LineClip::None, []() {
        return FontMeasureMetrics(30);
    }).lines;

    ASSERT_EQ(2, values.size());
    EXPECT_EQ("ab", values[0].text);
    EXPECT_EQ("a", values[1].text);
    EXPECT_EQ(0, values[0].y);
    EXPECT_EQ(30, values[1].y);
    EXPECT_EQ(Vector2(14, sut.font.Height()), values[0].size);
    EXPECT_EQ(Vector2(5, sut.font.Height()), values[1].size);
}
