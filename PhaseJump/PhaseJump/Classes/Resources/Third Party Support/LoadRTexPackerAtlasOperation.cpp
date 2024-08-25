#include "LoadRTexPackerAtlasOperation.h"
#include "AtlasTexture.h"
#include "Font.h"
#include "LoadResourcesModel.h"
#include "RTextPackerAtlasModelBuilder.h"
#include "SDLLoadGLTextureOperation.h"
#include "TextureAtlas.h"
#include "Utils.h"
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

SomeLoadResourcesOperation::Result LoadRTexPackerAtlasOperation::LoadResources() {
    FilePath filePath = info.filePath;

    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        PJLog("ERROR. Can't open file at: %s", info.filePath.c_str());
        return Failure();
    }

    try {
        json j;
        file >> j;

        auto model = RTextPackerAtlasModelBuilder().Build(j);

        String imagePath = model.atlas.imagePath;

        FilePath fullImagePath = info.filePath;
        fullImagePath.remove_filename();
        fullImagePath /= imagePath.c_str();

        // No id needed, we're not storing the parent texture in resources
        LoadResourceInfo loadTextureInfo(fullImagePath, "texture", "");
        auto loadTextureOperations = loadResourcesModel.MakeLoadOperations(loadTextureInfo);

        GUARDR(!IsEmpty(loadTextureOperations), Failure())

        auto& loadTextureOperation = *(*loadTextureOperations.begin());

        loadTextureOperation.Run();
        GUARDR(loadTextureOperation.result, Failure())
        GUARDR(loadTextureOperation.result.value().IsSuccess(), Failure())
        auto successValue = loadTextureOperation.result.value().SuccessValue();

        GUARDR(successValue && successValue.value().size(), Failure())

        SP<SomeTexture> texture =
            DCAST<SomeTexture>(successValue.value().loadedResources[0].resource);
        GUARDR(texture, Failure());

        Success result;

        auto textureAtlas = MAKE<TextureAtlas>();
        VectorList<RTexPackerAtlasModel::Char> chars;
        VectorList<SP<AtlasTexture>> charTextures;

        for (auto& sprite : model.sprites) {
            String alphaMode = texture->alphaMode;
            auto atlasTexture = MAKE<AtlasTexture>(
                texture, sprite.nameId, sprite.position, sprite.trimSize, sprite.trimOrigin,
                sprite.sourceSize, alphaMode
            );

            if (sprite._char) {
                chars.push_back(*sprite._char);
                charTextures.push_back(atlasTexture);
            }

            if (!model.atlas.isFont) {
                LoadedResource loadedResource(
                    filePath, "texture", sprite.nameId, SCAST<PJ::Base>(atlasTexture)
                );
                result.Add(loadedResource);
            }

            textureAtlas->Add(atlasTexture);
        }

        LoadedResource loadedAtlasResource(
            filePath, "texture.atlas", info.id, SCAST<PJ::Base>(textureAtlas)
        );
        result.Add(loadedAtlasResource);

        if (model.atlas.isFont) {
            auto font = MAKE<Font>();
            font->atlas = textureAtlas;

            for (int i = 0; i < chars.size(); i++) {
                auto& _char = chars[i];
                auto& charTexture = charTextures[i];

                Font::Glyph glyph;
                glyph.advanceX = _char.advanceX;
                glyph.size = charTexture->Size();
                glyph.offset = _char.offset;

                // FUTURE: support Unicode characters (ASCII for now)
                glyph.value = _char.value;
                glyph.texture = charTexture;

                font->glyphs[glyph.value] = glyph;
            }

            // rTexPacker doesn't give us any font metrics, so we have to calculate them
            int ascent = 0;
            int descent = 0;

            VectorList<char> descendingChars{ 'g', 'j', 'p', 'q', 'y' };

            for (auto& dc : descendingChars) {
                auto glyphI = font->glyphs.find(dc);
                GUARD_CONTINUE(glyphI != font->glyphs.end())

                auto& glyph = glyphI->second;
                descent = std::max(descent, glyph.offset.y);
            }

            constexpr int alphaCharCount = 26;

            char uppercaseChars[alphaCharCount]{};
            std::iota(std::begin(uppercaseChars), std::end(uppercaseChars), 'A');

            for (int aIndex = 0; aIndex < alphaCharCount; aIndex++) {
                auto upper = uppercaseChars[aIndex];

                auto glyphI = font->glyphs.find(upper);
                GUARD_CONTINUE(glyphI != font->glyphs.end())

                auto& glyph = glyphI->second;

                ascent = std::max(ascent, glyph.Height());
            }

            // These values are best guesses. Adjust as needed
            font->metrics.ascent = ascent;
            font->metrics.descent = descent;
            font->metrics.leading = (ascent + descent) + 5;

            LoadedResource loadedFontResource(filePath, "font", info.id, SCAST<PJ::Base>(font));
            result.Add(loadedFontResource);
        }

        return result;
    } catch (...) {
        PJLog("ERROR: Couldn't parse rTexPacker JSON");
    }

    return Failure();
}
