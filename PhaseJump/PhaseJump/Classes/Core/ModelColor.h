#pragma once

#include "Color.h"
#include <array>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/13/24

 Conversions are from:
 http://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c
 */
namespace PJ {
    enum class ColorModel { RGB, HSL, HSV };

    using RGBColor = Color;

    struct HSLColor {
        float h = 0;
        float s = 0;
        float l = 0;
        float a = 0;

        HSLColor(float h, float s, float l, float a) :
            h(h),
            s(s),
            l(l),
            a(a) {}
    };

    struct HSVColor {
        float h = 0;
        float s = 0;
        float v = 0;
        float a = 0;

        HSVColor(float h, float s, float v, float a) :
            h(h),
            s(s),
            v(v),
            a(a) {}
    };

    /// 4 color components, in a specific color model space
    class ModelColor {
    public:
        // TODO: do we need a guard for std::get? (try/catch?)
        std::variant<RGBColor, HSLColor, HSVColor> value;

        ModelColor() {}

        ModelColor(Color const& color);

        PJ::ColorModel ColorModel() const;

        void ToColorModel(PJ::ColorModel colorModel);
        ModelColor ToRGB() const;
        ModelColor ToHSL() const;
        ModelColor ToHSV() const;

        operator RGBColor() const {
            return std::get<RGBColor>(value);
        }

        operator HSLColor() const {
            return std::get<HSLColor>(value);
        }

        operator HSVColor() const {
            return std::get<HSVColor>(value);
        }

    protected:
        float HueToRGB(float p, float q, float t) const;
    };
} // namespace PJ
