#pragma once

#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 8/24/24
 */
namespace PJ {
    class Font;

    /// Defines the metrics for a measured line of text
    struct TextLineMetrics {
    public:
        struct CharMetric {
            String text;
            float advanceX = 0;
        };

        String text;

        /// Advance for each character on the line (allows for custom kerning for character pairs)
        VectorList<CharMetric> charMetrics;

        /// Line position (from top of ascent)
        float y = 0;

        /// Measured line size
        Vector2 size;

        /// Line start index in the source text
        size_t sourceIndex = 0;

        TextLineMetrics(float fontLeading) {
            size.y = fontLeading;
        }

        void Add(StringView _char, float advanceX) {
            CharMetric cm;
            cm.text = _char;
            cm.advanceX = advanceX;
            charMetrics.push_back(cm);

            text += _char;
        }
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

        TextMeasurer(Font& font) :
            font(font) {}

        VectorList<TextLineMetrics> MeasureLines(
            String text, Vector2 textSize, LineClip lineClip = LineClip::Hidden,
            MeasureMetricsFunc metricsFunc = MeasureMetricsFunc()
        );
    };
} // namespace PJ
