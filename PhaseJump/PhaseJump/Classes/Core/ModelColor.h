#pragma once

#include "Color.h"
#include <array>
#include <variant>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/1/25

 Reference:

 - http://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c
 */
namespace PJ {
    enum class ColorModelType { RGB, HSL, HSV };

    using RGBColor = Color;

    /// Hue saturation lightness color
    struct HSLColor {
        float h{};
        float s{};
        float l{};
        float a{};

        HSLColor(float h = 0, float s = 0, float l = 0, float a = 0) :
            h(h),
            s(s),
            l(l),
            a(a) {}
    };

    /// Hue saturation value color
    struct HSVColor {
        float h{};
        float s{};
        float v{};
        float a{};

        HSVColor(float h = 0, float s = 0, float v = 0, float a = 0) :
            h(h),
            s(s),
            v(v),
            a(a) {}
    };

    /// Contains color components, in a specific color model space
    class ModelColor {
    public:
        std::variant<RGBColor, HSLColor, HSVColor> value;

        ModelColor() {}

        ModelColor(Color const& color);

        /// @return Returns the color model type currently stored
        ColorModelType GetColorModelType() const;

        /// @return Returns this color converted to the specified color model type
        ModelColor Converted(ColorModelType colorModelType);

        /// @return Returns a RGB model color from this color model
        ModelColor ToRGB() const;

        /// @return Returns a HSL model color from this color model
        ModelColor ToHSL() const;

        /// @return Returns a HSV model color from this color model
        ModelColor ToHSV() const;

        /// @return Returns a safe color value, with a conversion operation if that color type isn't
        /// being stored
        template <class ColorType>
        ColorType SafeColorValue(std::function<ModelColor(ModelColor const&)> converter) const {
            auto result = std::get_if<ColorType>(&value);
            if (result) {
                return *result;
            }
            auto convertedModel = converter(*this);
            result = std::get_if<ColorType>(&convertedModel.value);
            if (result) {
                return *result;
            }
            return {};
        }

        /// @return Returns an RGB coolor
        operator RGBColor() const {
            return SafeColorValue<RGBColor>([](auto& color) { return color.ToRGB(); });
        }

        operator HSLColor() const {
            return SafeColorValue<HSLColor>([](auto& color) { return color.ToHSL(); });
        }

        operator HSVColor() const {
            return SafeColorValue<HSVColor>([](auto& color) { return color.ToHSV(); });
        }

    protected:
        float HueToRGB(float p, float q, float t) const;
    };
} // namespace PJ
