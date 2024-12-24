#pragma once

#include "AttributedString.h"
#include "TextMetrics.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/28/24
 */
namespace PJ {
    class Font;

    enum class TextWrap {
        /// Wrap by individual characters
        Character,

        /// Wrap words
        Word

        // FUTURE: support contextual grammar-type word wrap if needed
        // Grammar
    };

    enum class TextWordWrap {
        /// Split words when there isn't enough horizontal space
        Break,

        /// Never split individual words
        None

        // FUTURE: split, and add dash after first part of broken word
        // AddDash
    };

    /// Defines line clip behavior when text extends outside its available bounds
    enum class LineClip {
        /// Don't clip lines
        None,

        /// Clip partially hidden and fully hidden lines
        Partial,

        /// Clip fully hidden lines
        Hidden
    };

    /// Allows us to override the font's default metrics if needed
    struct FontMeasureMetrics {
        /// Distance from baseline to next baseline
        int leading = 0;

        FontMeasureMetrics(int leading) :
            leading(leading) {}
    };

    /// Measures glyph and line sizes for text
    struct TextMeasurer {
        using MeasureMetricsFunc = std::function<FontMeasureMetrics()>;

        Font& font;
        TextWrap textWrap;
        TextWordWrap textWordWrap;

        TextMeasurer(
            Font& font, TextWrap textWrap = TextWrap::Word,
            TextWordWrap textWordWrap = TextWordWrap::Break
        ) :
            font(font) {}

        TextMetrics Measure(
            AttributedString text, Vector2 textSize, LineClip lineClip = LineClip::Hidden,
            MeasureMetricsFunc metricsFunc = MeasureMetricsFunc()
        );
    };
} // namespace PJ
