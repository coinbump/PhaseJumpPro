#include "TextMeasurer.h"
#include "Font.h"

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

        auto charString = U32CharToString(u32[i]);
        GUARD_CONTINUE(charString.size() > 0)

        auto lineBreak = [&](int width) {
            result.push_back(line);
            TextLineMetrics newLine(font.Height());
            newLine.y = line.y + leading;

            // Don't add invisible characters
            if (width > 0) {
                newLine.text = charString;
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
            lineBreak(0);
            continue;
        }

        auto const& fontGlyphI = font.glyphs.find(charString);
        GUARD_CONTINUE(fontGlyphI != font.glyphs.end());

        auto& fontGlyph = fontGlyphI->second;

        auto width = fontGlyph.advanceX + fontGlyph.offset.x;
        if (line.size.x + width > textSize.x) {
            if (line.text.size() == 0) {
                PJLog("ERROR. No room to wrap text");
                return result;
            }

            lineBreak(width);
        } else {
            line.text += charString;
            line.size.x += width;
        }

        if (i == u32.size() - 1) {
            result.push_back(line);
        }
    }

    return result;
}
