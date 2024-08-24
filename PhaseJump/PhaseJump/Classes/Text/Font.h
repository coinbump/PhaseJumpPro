#pragma once

#include "Base.h"
#include "Tags.h"
#include "Vector2.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/24/24
 */
namespace PJ {
    class TextureAtlas;
    class SomeTexture;

    struct FontMetrics {
        /// Height of font above baseline
        int ascent = 0;

        /// Height of font below baseline
        int descent = 0;

        /// Distance from baseline to next baseline
        int leading = 0;
    };

    /// A font resource is defined by a texture atlas and a glyph map
    class Font : public Base {
    public:
        /// A font is composed of glyphs
        class Glyph {
        public:
            String value;

            /// Offset to render the glyph (example: j hangs behind character bounds)
            Vector2Int offset;

            /// Distance from glyph origin to next glyph
            int advanceX = 0;

            /// Texture to render the glyph
            SP<SomeTexture> texture;

            int Height() const;
        };

        /// Texture atlas that defines the glyph textures for the font
        SP<TextureAtlas> atlas;

        /// Map of UTF-8 string to glyph
        UnorderedMap<String, Glyph> glyphs;

        FontMetrics metrics;

        /// Attribute tags. Example: bold, italic, heavy, etc.
        TypeTagSet typeTags;

        int Height() const {
            return metrics.ascent + metrics.descent;
        }

        /// Returns true if this font can display all the glyphs in the string
        bool CanDisplay(String value);

        virtual ~Font() {}
    };
} // namespace PJ
