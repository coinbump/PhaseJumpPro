#include "Font.h"
#include "AtlasTexture.h"
#include "SomeTexture.h"
#include "StringUtils.h"

using namespace std;
using namespace PJ;

int Font::Glyph::Height() const {
    GUARDR(texture, 0);

    return texture->size.y;

    //    auto atlasTexture = As<AtlasTexture>(texture.get());
    //    GUARDR(atlasTexture, texture->size.y);
    //
    //    return atlasTexture->trueSize.y;
}

bool Font::CanDisplay(String text) {
    auto u32 = ToU32String(text);

    // FUTURE: support Unicode
    for (int i = 0; i < u32.size(); i++) {
        auto _char = U32CharToString(u32[i]);

        // ASCII Whitespace:
        //        if (_char.size() == 1) {
        //            if (_char[0] == ' ' || _char[0] == '\n' || _char[0] == '\r') { continue; }
        //        }

        if (glyphs.find(_char) == glyphs.end()) {
            return false;
        }
    }

    return true;
}
