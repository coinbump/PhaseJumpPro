#pragma once

#include "AttributedString.h"
#include "StringUtils.h"
#include "Vector2.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 11/28/24
 */
namespace PJ {
    /// Stores metrics for a measured line of text
    struct TextLineMetrics {
    public:
        struct CharMetric {
            String text;
            float advanceX = 0;
            VectorList<TextPart> attributes;
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

        void Add(StringView _char, float advanceX, VectorList<TextPart> attributes) {
            CharMetric cm;
            cm.text = _char;
            cm.advanceX = advanceX;
            cm.attributes = attributes;
            charMetrics.push_back(cm);

            text += _char;
        }
    };

    /// Stores metrics for a text renderer
    struct TextMetrics {
        VectorList<TextLineMetrics> lines;

        Vector2 CalculateSize() {
            return TextMetrics::CalculateSize(lines);
        }

        static Vector2 CalculateSize(VectorList<TextLineMetrics> const& lines) {
            GUARDR(!IsEmpty(lines), {})

            Vector2 result;

            std::for_each(lines.cbegin(), lines.cend(), [&](TextLineMetrics const& line) {
                result.x = std::max(result.x, line.size.x);
            });

            auto& lastLine = lines[lines.size() - 1];
            result.y = lastLine.y + lastLine.size.y;

            return result;
        }
    };
} // namespace PJ
