#include "gtest/gtest.h"
#include "Color.h"

using namespace PJ;
using namespace std;

namespace ColorTests {
}

using namespace ColorTests;

TEST(RGBAColor, Init_White)
{
    RGBAColor color32(255, 255, 255, 255);
    // color32.LogColorSchema();

    EXPECT_EQ(255, color32.r());
    EXPECT_EQ(255, color32.g());
    EXPECT_EQ(255, color32.b());
    EXPECT_EQ(255, color32.a());

    EXPECT_EQ(1.0f, color32.redFloat());
    EXPECT_EQ(1.0f, color32.greenFloat());
    EXPECT_EQ(1.0f, color32.blueFloat());
    EXPECT_EQ(1.0f, color32.alphaFloat());
}

TEST(RGBAColor, Init_Clear)
{
    RGBAColor color32(0, 0, 0, 0);
    EXPECT_EQ(0, color32.r());
    EXPECT_EQ(0, color32.g());
    EXPECT_EQ(0, color32.b());
    EXPECT_EQ(0, color32.a());

    EXPECT_EQ(0, color32.redFloat());
    EXPECT_EQ(0, color32.greenFloat());
    EXPECT_EQ(0, color32.blueFloat());
    EXPECT_EQ(0, color32.alphaFloat());
}

TEST(RGBAColor, Bytes)
{
    RGBAColor color32(1, 2, 3, 4);
    EXPECT_EQ(1, color32.r());
    EXPECT_EQ(2, color32.g());
    EXPECT_EQ(3, color32.b());
    EXPECT_EQ(4, color32.a());
}

TEST(Color, RGBAColorToColor)
{
    RGBAColor color32(255, 255, 255, 255);
    Color color(color32);

    EXPECT_EQ(1.0f, color.r);
    EXPECT_EQ(1.0f, color.g);
    EXPECT_EQ(1.0f, color.b);
    EXPECT_EQ(1.0f, color.a);
}

TEST(Color, ColorToColor32)
{
    Color color(0.5f, 0.5f, 0.5f, 1.0f);
    RGBAColor color32 = color;

    EXPECT_EQ(127, color32.r());
    EXPECT_EQ(127, color32.g());
    EXPECT_EQ(127, color32.b());
    EXPECT_EQ(255, color32.a());
}
