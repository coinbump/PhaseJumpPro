#include "TextMeasurer.h"
#include "Font.h"
#include "SomeTexture.h"

using namespace std;
using namespace PJ;

VectorList<TextLineMetrics> TextMeasurer::MeasureLines(
    String text, Vector2 textSize, LineClip lineClip, MeasureMetricsFunc metricsFunc
) {
    VectorList<TextLineMetrics> result;

    auto u32 = ToU32String(text);

    TextLineMetrics line(font.Height());

    int leading = font.metrics.leading;
    if (metricsFunc) {
        auto metrics = metricsFunc();
        leading = metrics.leading;
    }

    size_t sourceIndex = 0;

    // FUTURE: support Unicode
    for (int i = 0; i < u32.size(); i++, sourceIndex++) {
        float lineBottom = line.y + line.size.y;
        if (lineBottom > textSize.y) {
            switch (lineClip) {
            case LineClip::None:
                break;
            case LineClip::Partial:
                return result;
                break;
            case LineClip::Hidden:
                if (line.y >= textSize.y) {
                    return result;
                }
                break;
            }
        }

        uint32_t u32Char = u32[i];
        auto charString = U32CharToString(u32Char);
        GUARD_CONTINUE(charString.size() > 0)

        auto lineBreak = [&](int width, int advanceX) {
            result.push_back(line);
            TextLineMetrics newLine(font.Height());
            newLine.y = line.y + leading;

            // Don't add invisible characters
            if (width > 0) {
                newLine.Add(charString, advanceX);
                newLine.size.x = width;
                newLine.sourceIndex = sourceIndex;
            } else {
                // Skip the newline character
                newLine.sourceIndex = sourceIndex + 1;
            }
            line = newLine;
        };

        if ('\n' == charString[0]) {
            // This is a line break character
            lineBreak(0, 0);
            continue;
        }

        auto const& fontGlyphI = font.glyphs.find(u32Char);
        GUARD_CONTINUE(fontGlyphI != font.glyphs.end());

        auto& fontGlyph = fontGlyphI->second;

        Vector2 pos;

        int advanceX = fontGlyph.advanceX;
        if (line.charMetrics.size() > 0) {
            auto const& prevCharMetric = line.charMetrics[line.charMetrics.size() - 1];
            uint32_t prevU32 = ToU32String(prevCharMetric.text)[0];
            FontMetrics::KernPair kernPair = std::make_pair(prevU32, u32Char);
            auto kernI = font.metrics.kern.find(kernPair);
            if (kernI != font.metrics.kern.end()) {
                advanceX += kernI->second;
            }
        }

        auto width = fontGlyph.size.x + fontGlyph.offset.x;
        if (line.size.x + width > textSize.x) {
            if (line.charMetrics.size() == 0) {
                PJLog("ERROR. No room to wrap text");
                return result;
            }

            lineBreak(width, fontGlyph.advanceX);
        } else {
            line.Add(charString, advanceX);
            line.size.x += advanceX;
        }

        if (i == u32.size() - 1) {
            result.push_back(line);
        }
    }

    return result;
}
