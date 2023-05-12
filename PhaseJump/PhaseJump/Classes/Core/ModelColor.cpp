#include "ModelColor.h"
#include "Color.h"

using namespace std;
using namespace PJ;

ModelColor::ModelColor(Color const& color)
{
    value = color;
}

PJ::ColorModel ModelColor::ColorModel() const {
    if (std::holds_alternative<RGBColor>(value)) {
        return ColorModel::RGB;
    } else if (std::holds_alternative<HSLColor>(value)) {
        return ColorModel::HSL;
    } else if (std::holds_alternative<HSVColor>(value)) {
        return ColorModel::HSV;
    }

    return ColorModel::RGB;
}

void ModelColor::ToColorModel(PJ::ColorModel colorModel) {
    switch (colorModel) {
        case ColorModel::RGB:
            ToRGB();
            break;
        case ColorModel::HSL:
            ToHSL();
            break;
        case ColorModel::HSV:
            ToHSV();
            break;
    }
}

ModelColor ModelColor::ToHSV() const {
    switch (ColorModel()) {
        case ColorModel::RGB: {
            auto rgb = std::get<RGBColor>(value);
            auto r = rgb.r;
            auto g = rgb.g;
            auto b = rgb.b;

            float max = std::max(std::max(r, g), b);
            float min = std::min(std::min(r, g), b);
            float h, s, v = max;

            float d = max - min;
            s = max == 0 ? 0 : d / max;

            if (max == min) {
                h = 0; // Achromatic
            } else {
                if (max == r) {
                    h = (g - b) / d + (g < b ? 6.0f : 0);
                } else if (max == g) {
                    h = (b - r) / d + 2.0f;
                } else {
                    h = (r - g) / d + 4.0f;
                }
                h /= 6;
            }

            ModelColor result;
            result.value = HSVColor(h, s, v, rgb.a);
            return result;
            break;
        }
        case ColorModel::HSL: {
            auto rgb = ToRGB();
            auto result = rgb.ToHSL();
            return result;
            break;
        }
        case ColorModel::HSV:
            break;
    }

    return *this;
}

ModelColor ModelColor::ToRGB() const {
    switch (ColorModel()) {
        case ColorModel::HSL: {
            auto hsl = std::get<HSLColor>(value);

            float r, g, b;

            if (0 == hsl.s) {
                r = g = b = hsl.l; // Achromatic
            }
            else {
                float q = hsl.l < 0.5f ? hsl.l * (1 + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
                float p = 2 * hsl.l - q;
                r = HueToRGB(p, q, hsl.h + 1.0f/3.0f);
                g = HueToRGB(p, q, hsl.h);
                b = HueToRGB(p, q, hsl.h - 1.0f/3.0f);
            }

            ModelColor result;
            result.value = RGBColor(r, g, b, hsl.a);
            return result;
            break;
        }
        case ColorModel::HSV: {
            auto hsv = std::get<HSVColor>(value);
            auto h = hsv.h;
            auto s = hsv.s;
            auto v = hsv.v;

            float r = 0, g = 0, b = 0;

            int i = (int)floor(h * 6.0f);
            float f = h * 6.0f - i;
            float p = v * (1.0f - s);
            float q = v * (1.0f - f * s);
            float t = v * (1.0f - (1.0f - f) * s);

            switch (i % 6) {
                case 0: r = v; g = t; b = p; break;
                case 1: r = q; g = v; b = p; break;
                case 2: r = p; g = v; b = t; break;
                case 3: r = p; g = q; b = v; break;
                case 4: r = t; g = p; b = v; break;
                case 5: r = v; g = p; b = q; break;
            }

            ModelColor result;
            result = RGBColor(r, g, b, hsv.a);
            return result;
            break;
        }
        case ColorModel::RGB:
            break;
    }

    return *this;
}

float ModelColor::HueToRGB(float p, float q, float t) const {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f/2.0f) return q;
    if (t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
    return p;
}

ModelColor ModelColor::ToHSL() const {
    switch (ColorModel()) {
        case ColorModel::RGB: {
            auto rgb = std::get<RGBColor>(value);

            float _max = fmaxf(fmaxf(rgb.r, rgb.g), rgb.b);
            float _min = fminf(fminf(rgb.r, rgb.g), rgb.b);
            float h, s, l;
            h = 0;
            l = (_max + _min) / 2;

            if (_max == _min) {
                h = 0;
                s = 0;    // Achromatic
            }
            else {
                float d = _max - _min;
                s = l > 0.5f ? d / (2 - _max - _min) : d / (_max + _min);

                if (_max == rgb.r) {
                    h = (rgb.g - rgb.b) / d + (rgb.g < rgb.b ? 6 : 0);
                }
                else if (_max == rgb.g) {
                    h = (rgb.b - rgb.r) / d + 2;
                }
                else if (_max == rgb.b) {
                    h = (rgb.r - rgb.g) / d + 4;
                }
                h /= 6.0f;
            }

            ModelColor result;
            result.value = HSLColor(h, s, l, rgb.a);
            return result;
            break;
        }
        case ColorModel::HSV: {
            auto rgb = ToRGB();
            auto result = rgb.ToHSL();
            return result;
        }
        case ColorModel::HSL:
            break;
    }

    return *this;
}
