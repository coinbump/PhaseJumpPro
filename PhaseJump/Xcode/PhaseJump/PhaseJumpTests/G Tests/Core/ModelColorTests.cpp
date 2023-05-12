#include "gtest/gtest.h"
#include "ModelColor.h"

using namespace PJ;
using namespace std;

namespace ModelColorTests {
}

using namespace ModelColorTests;

TEST(ModelColor, Unit_Tests) {
    {
        Color color(.5f, .5f, .5f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(.1f, .3f, .75f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(.15f, .35f, .2f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(.8f, .8f, .05f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(1.0f, .3f, .4f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(128.0f/255.0f, 0, 0, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();

        auto hsl = std::get<HSLColor>(modelColor.value);

        EXPECT_NEAR(hsl.h, 0/100.0f, .001f);
        EXPECT_NEAR(hsl.s, 100.0f/100.0f, .001f);
        EXPECT_NEAR(hsl.l, 25.1f/100.0f, .001f);
    }
    {
        Color color(128.0f/255.0f, 75.0f/255.0f, 125.0f/255.0f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSL();

        auto hsl = std::get<HSLColor>(modelColor.value);

        EXPECT_NEAR(hsl.h, 303/360.0f, .002f);
        EXPECT_NEAR(hsl.s, 26.1f/100.0f, .001f);
        EXPECT_NEAR(hsl.l, 39.8f/100.0f, .001f);
    }
}

TEST(ModelColor, TestHSV) {
    {
        Color color(.5f, .5f, .5f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSV();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(.25f, .35f, .75f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSV();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(.25f, .35f, .75f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSV();
        modelColor = modelColor.ToHSL();
        modelColor = modelColor.ToRGB();

        auto rgb = std::get<RGBColor>(modelColor.value);

        EXPECT_NEAR(rgb.r, color.r, .001f);
        EXPECT_NEAR(rgb.g, color.g, .001f);
        EXPECT_NEAR(rgb.b, color.b, .001f);
    }
    {
        Color color(128.0f/255.0f, 75.0f/255.0f, 125.0f/255.0f, 1.0f);
        ModelColor modelColor(color);
        modelColor = modelColor.ToHSV();

        auto hsv = std::get<HSVColor>(modelColor.value);

        EXPECT_NEAR(hsv.h, .84276, .001f);
        EXPECT_NEAR(hsv.s, .4141f, .001f);
        EXPECT_NEAR(hsv.v, .502f, .001f);
    }
}
