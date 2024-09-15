#ifndef PJCOLOR_H
#define PJCOLOR_H

#include "FloatMath.h"
#include "RGBAColor.h"
#include "StringUtils.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Stores RGBA as normalized float components (0-1.0)
    struct Color {
        using This = Color;

        float r = 0;
        float g = 0;
        float b = 0;
        float a = 1;

        constexpr Color() {}

        constexpr Color(float red, float green, float blue, float alpha) :
            r(red),
            g(green),
            b(blue),
            a(alpha) {}

        Color(RGBAColor color) :
            r(color.redFloat()),
            g(color.greenFloat()),
            b(color.blueFloat()),
            a(color.alphaFloat()) {}

        operator RGBAColor() const {
            return RGBAColor(
                (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f), (int)(a * 255.0f)
            );
        }

        bool operator==(Color const& rhs) const {
            return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
        }

        constexpr bool IsOpaque() const {
            return a >= 1.0f;
        }

        /// Returns this color with a modified color value
        constexpr This WithRed(float value, bool clamp = true) const {
            This result = *this;
            result.r = clamp ? std::clamp(value, 0.0f, 1.0f) : value;
            return result;
        }

        /// Returns this color with a modified color value
        constexpr This WithGreen(float value, bool clamp = true) const {
            This result = *this;
            result.g = clamp ? std::clamp(value, 0.0f, 1.0f) : value;
            return result;
        }

        /// Returns this color with a modified color value
        constexpr This WithBlue(float value, bool clamp = true) const {
            This result = *this;
            result.b = clamp ? std::clamp(value, 0.0f, 1.0f) : value;
            return result;
        }

        /// Returns this color with a modified alpha value
        constexpr This WithAlpha(float value, bool clamp = true) const {
            This result = *this;
            result.a = clamp ? std::clamp(value, 0.0f, 1.0f) : value;
            return result;
        }

        /// Multiple the colors by value and optionally clamp the result
        This WithColorsMult(float value, bool clamp = true) const {
            This result = *this;
            result = result.WithRed(result.r * value, clamp);
            result = result.WithGreen(result.g * value, clamp);
            result = result.WithBlue(result.b * value, clamp);
            return result;
        }

        // MARK: StringConvertible

        String ToString() const {
            // TODO: create ToString formatter with pair<String, String>
            return "Red: " + MakeString(r) + ", Green: " + MakeString(g) +
                   ", Blue: " + MakeString(b) + ", Alpha: " + MakeString(a);
        }

        friend std::ostream& operator<<(std::ostream&, Color const& value);

        /*
         ADDITIONAL COLOR REFERENCES:
         iOS/MacOS system colors:
         https://developer.apple.com/design/human-interface-guidelines/color Windows system colors:
         https://learn.microsoft.com/en-us/uwp/api/windows.ui.colors
         */

        // MARK: Constants

        static const Color red;
        static const Color green;
        static const Color blue;
        static const Color black;
        static const Color clear;
        static const Color gray;
        static const Color white;
        static const Color yellow;
    };

    // MARK: Stream friend operators

    std::ostream& operator<<(std::ostream& os, Color const& value);

} // namespace PJ

#endif
