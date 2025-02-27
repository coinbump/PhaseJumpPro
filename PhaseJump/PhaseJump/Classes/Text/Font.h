#pragma once

#include "Base.h"
#include "OrderedMap.h"
#include "Tags.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/25/24
 */
namespace PJ {
    class TextureAtlas;
    class SomeTexture;

    /// Defines parameters for finding a font from resources
    struct FontSpec {
        String resourceId;
        String fontName;
        float size = 0;
    };

    /// Stores font metrics
    struct FontMetrics {
        using KernPair = std::pair<uint32_t, uint32_t>;

        /// Height of font above baseline
        int ascent = 0;

        /// Height of font below baseline
        int descent = 0;

        /// Distance from baseline to next baseline
        int leading = 0;

        /// Defines custom kerning for pairs of characters that appear together
        OrderedMap<KernPair, int> kern;
    };

    /// A font resource is defined by a texture atlas and a glyph map
    class Font : public Base {
    public:
        /// A font is composed of glyphs
        class Glyph {
        public:
            uint32_t value = 0;

            /// Offset to render the glyph (example: j hangs behind character bounds)
            Vector2Int offset;

            /// Size of the glyph, so we can access this without using the texture
            Vector2Int size;

            /// Distance from glyph origin to next glyph
            int advanceX = 0;

            /// Texture to render the glyph
            SP<SomeTexture> texture;

            int Height() const;
        };

        /// Texture atlas that defines the glyph textures for the font
        SP<TextureAtlas> atlas;

        String name;
        float size = 0;

        /// Map of UTF-8 string to glyph
        UnorderedMap<uint32_t, Glyph> glyphs;

        FontMetrics metrics;

        /// Attribute tags. Example: bold, italic, heavy, etc.
        TypeTagSet typeTags;

        struct Config {
            String name;
            float size{};
        };

        Font(Config config) :
            name(config.name),
            size(config.size) {}

        int Height() const {
            return metrics.ascent + metrics.descent;
        }

        /// @return Returns true if this font can display all the glyphs in the string
        bool CanDisplay(String value);

        virtual ~Font() {}
    };
} // namespace PJ
