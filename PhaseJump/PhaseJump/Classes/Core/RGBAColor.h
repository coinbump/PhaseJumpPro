#pragma once

#include "Dev.h"
#include "FloatMath.h"
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

    /// Stores RGBA as single value
    template <class Type>
    struct ComponentColor {
        Type value = 0;

        ComponentColor(Type value = {}) :
            value(value) {}

        ComponentColor(
            ComponentColorSchema const& schema, int red, int green, int blue, int alpha = 255
        ) {
            value = ValueFromRGBA(schema, red, green, blue, alpha);
        }

        ComponentColor(
            ComponentColorSchema const& schema, float red, float green, float blue,
            float alpha = 1.0f
        ) {
            value =
                ValueFromRGBA(schema, red * 255.0f, green * 255.0f, blue * 255.0f, alpha * 255.0f);
        }

        operator Type() const {
            return value;
        }

        Type
        ValueFromRGBA(ComponentColorSchema const& schema, int red, int green, int blue, int alpha) {
            return ((Type)red << schema.redShift) | ((Type)green << schema.greenShift) |
                   ((Type)blue << schema.blueShift) | ((Type)alpha << schema.alphaShift);
        }

        bool IsOpaque(ComponentColorSchema const& schema) const {
            return alphaFloat(schema) >= 1.0f;
        }

        int r(ComponentColorSchema const& schema) const {
            return ((value & schema.redMask) >> schema.redShift);
        }

        int g(ComponentColorSchema const& schema) const {
            return ((value & schema.greenMask) >> schema.greenShift);
        }

        int b(ComponentColorSchema const& schema) const {
            return ((value & schema.blueMask) >> schema.blueShift);
        }

        int a(ComponentColorSchema const& schema) const {
            /// XRGB formats don't store an alpha channel
            GUARDR(schema.alphaMask, 255)

            return ((value & schema.alphaMask) >> schema.alphaShift);
        }

        float redFloat(ComponentColorSchema const& schema) const {
            return ((float)r(schema)) / 255.0f;
        }

        float greenFloat(ComponentColorSchema const& schema) const {
            return ((float)g(schema)) / 255.0f;
        }

        float blueFloat(ComponentColorSchema const& schema) const {
            return ((float)b(schema)) / 255.0f;
        }

        float alphaFloat(ComponentColorSchema const& schema) const {
            return ((float)a(schema)) / 255.0f;
        }

        template <class ComponentColor>
        ComponentColor ConvertTo(ComponentColorSchema const& schema) const {
            return ComponentColor(
                redFloat(schema), greenFloat(schema), blueFloat(schema), alphaFloat(schema)
            );
        }
    };

    /// Stores RGBA values in a 32 bit value
    using ComponentColor32 = ComponentColor<uint32_t>;

    /// Component color with a concrete schema
    template <ComponentColorSchema const& ColorSchema>
    struct ConcreteColor32 : public ComponentColor32 {
    public:
        using Base = ComponentColor32;
        using Type = uint32_t;

        ConcreteColor32(Type value = {}) :
            Base(value) {}

        ConcreteColor32(int red, int green, int blue, int alpha = 255) :
            Base(ColorSchema, red, green, blue, alpha) {}

        ConcreteColor32(float red, float green, float blue, float alpha = 1.0f) :
            Base(ColorSchema, red, green, blue, alpha) {}

        constexpr uint32_t ValueFromRGBA(int red, int green, int blue, int alpha) {
            return ComponentColor32::ValueFromRGBA(ColorSchema, red, green, blue, alpha);
        }

        constexpr bool IsOpaque() const {
            return ComponentColor32::IsOpaque(ColorSchema);
        }

        constexpr int r() const {
            return ComponentColor32::r(ColorSchema);
        }

        constexpr int g() const {
            return ComponentColor32::g(ColorSchema);
        }

        constexpr int b() const {
            return ComponentColor32::b(ColorSchema);
        }

        constexpr int a() const {
            return ComponentColor32::a(ColorSchema);
        }

        constexpr float redFloat() const {
            return ((float)r()) / 255.0f;
        }

        constexpr float greenFloat() const {
            return ((float)g()) / 255.0f;
        }

        constexpr float blueFloat() const {
            return ((float)b()) / 255.0f;
        }

        constexpr float alphaFloat() const {
            return ((float)a()) / 255.0f;
        }
    };

    /// Stores 32-bit color as RGBA
    using RGBAColor = ConcreteColor32<ComponentColorSchema::rgba>;

    /// Stores 32-bit color as BGRAColor
    using BGRAColor = ConcreteColor32<ComponentColorSchema::bgra>;

    using Color32 = RGBAColor;
} // namespace PJ
