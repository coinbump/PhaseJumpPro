#ifndef PJCOLOR_H
#define PJCOLOR_H

#include "FloatMath.h"
#include "Color32.h"
#include "_String.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    /// Stores RGBA as normalized float components (0-1.0)
    struct Color
    {
        float r = 0;
        float g = 0;
        float b = 0;
        float a = 0;

        Color() {
        }

        Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha)
        {
        }

        Color(Color32 color) : r(color.redFloat()), g(color.greenFloat()), b(color.blueFloat()), a(color.alphaFloat())
        {
        }

        operator Color32() const {
            return Color32((int)(r*255.0f), (int)(g*255.0f), (int)(b*255.0f), (int)(a*255.0f));
        }

        Color WithAlpha(float a) const {
            Color result = *this;
            result.a = a;
            return result;
        }
        
        // MARK: StringConvertible

        String ToString() const {
            return "Red: " + String(r) + " Green: " + String(g) + " Blue: " + String(b) + " Alpha: " + String(a);
        }

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
}

#endif
