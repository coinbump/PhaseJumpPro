#ifndef PJCOLOR_H
#define PJCOLOR_H

#include "StringConvertible.h"
#include "FloatMath.h"
#include "SomeVector.h"
#include "Color32.h"
#include "_String.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    struct Color : public SomeVector<FloatMath, 4>, public StringConvertible
    {
        Color(float red, float green, float blue, float alpha)
        {
            this->r() = red;
            this->g() = green;
            this->b() = blue;
            this->a() = alpha;
        }

        Color(Color32 color)
        {
            this->r() = color.redFloat();
            this->g() = color.greenFloat();
            this->b() = color.blueFloat();
            this->a() = color.alphaFloat();
        }

        operator Color32() const {
            return Color32((int)(r()*255.0f), (int)(g()*255.0f), (int)(b()*255.0f), (int)(a()*255.0f));
        }

        float r() const { return (*this)[0]; }
        float g() const { return (*this)[1]; }
        float b() const { return (*this)[2]; }
        float a() const { return (*this)[3]; }
        float& r() { return (*this)[0]; }
        float& g() { return (*this)[1]; }
        float& b() { return (*this)[2]; }
        float& a() { return (*this)[3]; }

        float red() const { return r(); }
        float green() const { return g(); }
        float blue() const { return b(); }
        float alpha() const { return a(); }
        float& red() { return r(); }
        float& green() { return g(); }
        float& blue() { return b(); }
        float& alpha() { return a(); }

        // MARK: StringConvertible

        String ToString() const override {
            return "Red: " + String(r()) + " Green: " + String(g()) + " Blue: " + String(b()) + " Alpha: " + String(a());
        }

        // MARK: Constants

        static const Color redColor;
        static const Color greenColor;
        static const Color blueColor;
        static const Color black;
        static const Color clear;
        static const Color gray;
        static const Color white;
        static const Color yellow;
    };
}

#endif
