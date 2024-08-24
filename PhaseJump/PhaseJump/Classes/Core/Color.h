#ifndef PJCOLOR_H
#define PJCOLOR_H

#include "_String.h"
#include "FloatMath.h"
#include "RGBAColor.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 6/8/24
 */
namespace PJ {
    /// Stores RGBA as normalized float components (0-1.0)
    struct Color {
        float r = 0;
        float g = 0;
        float b = 0;
        float a = 0;

        Color() {}

        Color(float red, float green, float blue, float alpha) :
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

        bool IsOpaque() const {
            return a >= 1.0f;
        }

        Color WithAlpha(float a) const {
            Color result = *this;
            result.a = a;
            return result;
        }

        // MARK: StringConvertible

        String ToString() const {
            // TODO: create ToString formatter with pair<String, String>
            return "Red: " + String(r) + ", Green: " + String(g) + ", Blue: " + String(b) +
                   ", Alpha: " + String(a);
        }

        friend std::ostream& operator<<(std::ostream&, Color const& value);

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
