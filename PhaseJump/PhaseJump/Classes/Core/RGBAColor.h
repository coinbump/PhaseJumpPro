#ifndef PJCOLOR32_H
#define PJCOLOR32_H

#include "FloatMath.h"
#include "Log.h"
#include <stdint.h>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    /// Specifies the mask and bitshift schema for an N byte-color
    struct ComponentColorSchema {
        uint32_t redMask;
        uint32_t greenMask;
        uint32_t blueMask;
        uint32_t alphaMask;

        uint32_t redShift;
        uint32_t greenShift;
        uint32_t blueShift;
        uint32_t alphaShift;

        ComponentColorSchema(
            uint32_t redMask, uint32_t greenMask, uint32_t blueMask, uint32_t alphaMask,

            uint32_t redShift, uint32_t greenShift, uint32_t blueShift, uint32_t alphaShift
        ) :
            redMask(redMask),
            greenMask(greenMask),
            blueMask(blueMask),
            alphaMask(alphaMask),

            redShift(redShift),
            greenShift(greenShift),
            blueShift(blueShift),
            alphaShift(alphaShift) {}

        static const ComponentColorSchema rgba;
        static const ComponentColorSchema bgra;
    };

    /// Stores RGBA as a 32-bit value, one byte for each color component
    template <ComponentColorSchema const& ColorSchema>
    struct ComponentColor32 {
        uint32_t value = 0;

        ComponentColor32() :
            value(0) {}

        ComponentColor32(int red, int green, int blue, int alpha = 255) {
            value = ValueFromRGBA(red, green, blue, alpha);
        }

        ComponentColor32(float red, float green, float blue, float alpha = 1.0f) {
            value = ValueFromRGBA(red * 255.0f, green * 255.0f, blue * 255.0f, alpha * 255.0f);
        }

        operator uint32_t() const {
            return value;
        }

        uint32_t ValueFromRGBA(int red, int green, int blue, int alpha) {
            return ((uint32_t)red << ColorSchema.redShift) |
                   ((uint32_t)green << ColorSchema.greenShift) |
                   ((uint32_t)blue << ColorSchema.blueShift) |
                   ((uint32_t)alpha << ColorSchema.alphaShift);
        }

        bool IsOpaque() const {
            return alphaFloat() >= 1.0f;
        }

        int r() const {
            return ((value & ColorSchema.redMask) >> ColorSchema.redShift);
        }

        int g() const {
            return ((value & ColorSchema.greenMask) >> ColorSchema.greenShift);
        }

        int b() const {
            return ((value & ColorSchema.blueMask) >> ColorSchema.blueShift);
        }

        int a() const {
            return ((value & ColorSchema.alphaMask) >> ColorSchema.alphaShift);
        }

        float redFloat() const {
            return ((float)r()) / 255.0f;
        }

        float greenFloat() const {
            return ((float)g()) / 255.0f;
        }

        float blueFloat() const {
            return ((float)b()) / 255.0f;
        }

        float alphaFloat() const {
            return ((float)a()) / 255.0f;
        }

        template <ComponentColorSchema const& ResultColorSchema>
        operator ComponentColor32<ResultColorSchema>() const {
            ComponentColor32<ResultColorSchema> result;
            result.value = result.ValueFromRGBA(r(), g(), b(), a());
            return result;
        }

        void LogColorSchema() {
            PJLog("redMask: %u", ColorSchema.redMask);
            PJLog("greenMask: %u", ColorSchema.greenMask);
            PJLog("blueMask: %u", ColorSchema.blueMask);
            PJLog("alphaMask: %u", ColorSchema.alphaMask);

            PJLog("redShift: %u", ColorSchema.redShift);
            PJLog("greenShift: %u", ColorSchema.greenShift);
            PJLog("blueShift:%u", ColorSchema.blueShift);
            PJLog("alphaShift: %u", ColorSchema.alphaShift);
        }
    };

    using RGBAColor = ComponentColor32<ComponentColorSchema::rgba>;
    using Color32 = RGBAColor;
    using BGRAColor = ComponentColor32<ComponentColorSchema::bgra>;
} // namespace PJ

#endif
