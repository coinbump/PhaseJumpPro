#ifndef PJCOLOR32_H
#define PJCOLOR32_H

#include "FloatMath.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    /// Stores RGBA as a 32-bit value, one byte for each color component
    struct Color32 {
        uint32_t value;

        operator uint32_t&() {
            return value;
        }

        operator uint32_t() const {
            return value;
        }

        Color32(int red, int green, int blue, int alpha)
        {
            value = ValueFromRGBA(red, green, blue, alpha);
        }

        Color32(float red, float green, float blue, float alpha)
        {
            value = ValueFromRGBA(red * 255.0f, green * 255.0f, blue * 255.0f, alpha * 255.0f);
        }

        uint32_t ValueFromRGBA(int red, int green, int blue, int alpha)
        {
            return ((uint32_t) red << 24) | ((uint32_t) green << 16) | ((uint32_t) blue << 8) | ((uint32_t) alpha);
        }

        int r() const { return ((value & 0xFF000000) >> 24); }
        int g() const { return ((value & 0x00FF0000) >> 16); }
        int b() const { return ((value & 0x0000FF00) >> 8); }
        int a() const { return ((value & 0x000000FF)); }

        float redFloat() const { return ((float)r())/255.0f; }
        float greenFloat() const { return ((float)g())/255.0f; }
        float blueFloat() const { return ((float)b())/255.0f; }
        float alphaFloat() const { return ((float)a())/255.0f; }
    };
}

#endif
