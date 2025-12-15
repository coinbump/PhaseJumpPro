#pragma once

#include "Color.h"
#include "StringUtils.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/17/24
 */
namespace PJ {
    class SomeTexture;

    /// Common text style ids
    namespace TextStyleId {
        auto constexpr Bold = "bold";
        auto constexpr Italic = "italic";
        auto constexpr Underline = "underline";
        auto constexpr Strikethrough = "strike";
        auto constexpr Caps = "caps";
    } // namespace TextStyleId

    /// Common text part types
    namespace TextPartType {
        auto constexpr Text = "text";
        auto constexpr AttributeColor = "attribute.color";
    } // namespace TextPartType

    /// Adds additional metadata to texture
    class Image {
    public:
        SP<SomeTexture> texture;
        Vector2 size;
    };

    /// A part of an attributed text string
    struct TextPart {
        String type;
        String text;

        /// If > 0, this is an attribute with a start and end
        int attributeLength{};

        String fontName;
        float fontSize;

        Color color;

        bool IsAttribute() const {
            return attributeLength > 0;
        }

        // FUTURE: support inline images
        // FUTURE: Image image;

        // FUTURE: support paragraph styles + lookup
        // String styleId;
    };

    /// Text with formatting attributes
    class AttributedString {
    protected:
        using PartList = VectorList<TextPart>;

        void ApplyAttribute(TextPart part, int start, int length);
        void InsertPart(TextPart part, int pos);

        PartList parts;

    public:
        struct Config {
            String text;
        };

        AttributedString(Config const& config = {});

        AttributedString(String value) :
            AttributedString({ .text = value }) {}

        AttributedString(const char* value) :
            AttributedString({ .text = String(value) }) {}

        using This = AttributedString;

        String PlainText() const;

        /// Applies a color attribute to the specified range
        This& ApplyColor(Color value, int start, int length);

        /// Removes all non-text attributes
        This& RemoveAllAttributes();

        /// @return Returns a list of the active attributes at this position
        VectorList<TextPart> Attributes(int pos) const;

        /// @return Returns the color at the specified position, or returns the default if the text
        /// has no specified color
        Color GetColor(int pos, Color defaultValue = Color::black) const;

        /// Sets the plain text value, replacing any existing values
        void SetPlainText(String value);

        size_t PartCount() const {
            return parts.size();
        }

        // FUTURE: void EraseAttributes(int start, int end);
    };
} // namespace PJ
