#include "LoadBitmapFontOperation.h"
#include "AtlasTexture.h"
#include "CollectionUtils.h"
#include "Font.h"
#include "LoadResourcesModel.h"
#include "RTextPackerAtlasModelBuilder.h"
#include "SDLLoadGLTextureOperation.h"
#include "StringUtils.h"
#include "TextureAtlas.h"
#include "Utils.h"
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

Tags ReadLine(String line) {
    Tags result;

    auto keyValueComponents = ComponentsSeparatedBy(line, ' ');
    for (int i = 0; i < keyValueComponents.size(); i++) {
        auto& kv = keyValueComponents[i];
        auto components = ComponentsSeparatedBy(kv, '=');
        GUARD_CONTINUE(components.size() == 2)

        auto first = components[0];
        auto second = components[1];

        /// Get rid of these characters
        RemoveIf(second, [](char c) { return c == '\r'; });

        if (StartsWith(second, "\"")) {
            // String had 1 or more spaces inside, and has been split
            if (!EndsWith(second, "\"")) {
                i++;
                while (i < keyValueComponents.size()) {
                    auto next = keyValueComponents[i];
                    second += " ";
                    second += next;

                    if (EndsWith(second, "\"")) {
                        break;
                    }

                    i++;
                }
            }
            RemoveIf(second, [](char _char) { return _char == '"'; });

            result.Add(first, second);
        } else if (IsNumericInt(second)) {
            int intValue = TypeValue<int>(second);
            result.Add(first, intValue);
        } else {
            result.Add(first, second);
        }
    }

    return result;
}

Tags ReadLine(ifstream& file) {
    String line;
    std::getline(file, line);

    return ReadLine(line);
}

SomeLoadResourcesOperation::Result LoadBitmapFontOperation::LoadResources() {
    FilePath filePath = info.filePath;

    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        PJLog("ERROR. Can't open file at: %s", info.filePath.c_str());
        return Failure();
    }

    Tags tags;

    try {
        Tags infoTags = ReadLine(file);
        Tags commonTags = ReadLine(file);
        Tags pageTags = ReadLine(file);
        Tags charsTags = ReadLine(file);

        List<Tags> charLines;
        List<Tags> kernLines;

        String line;
        while (getline(file, line)) {
            auto rt = ReadLine(line);
            if (StartsWith(line, "kernings")) {
                continue;
            } else if (StartsWith(line, "kerning")) {
                kernLines.push_back(rt);
            } else if (StartsWith(line, "char")) {
                charLines.push_back(rt);
            }
        }

        String imagePath = pageTags.SafeValue<String>("file");

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

        auto font = MAKE<Font>();
        font->atlas = textureAtlas;

        for (auto& sprite : charLines) {
            // cout << sprite.ToString();

            String alphaMode = texture->alphaMode;
            Vector2Int size(sprite.SafeValue<int>("width"), sprite.SafeValue<int>("height"));
            Vector2Int pos(sprite.SafeValue<int>("x"), sprite.SafeValue<int>("y"));
            int id = sprite.SafeValue<int>("id");
            auto atlasTexture = MAKE<AtlasTexture>(
                texture, MakeString(id), pos, size, Vector2Int::zero, size, alphaMode
            );

            textureAtlas->Add(atlasTexture);

            Font::Glyph glyph;
            glyph.advanceX = sprite.SafeValue<int>("xadvance");
            glyph.size = size;
            glyph.offset =
                Vector2Int(sprite.SafeValue<int>("xoffset"), sprite.SafeValue<int>("yoffset"));

            glyph.value = id;
            glyph.texture = atlasTexture;

            font->glyphs[glyph.value] = glyph;

            font->metrics.ascent = commonTags.SafeValue<int>("base");
            font->metrics.descent = 0; // Descent isn't given by Bitmap Font Generator
            font->metrics.leading = commonTags.SafeValue<int>("lineHeight");
        }

        for (auto& kern : kernLines) {
            uint32_t kFirst = kern.SafeValue<int>("first");
            uint32_t kSecond = kern.SafeValue<int>("second");
            int amount = kern.SafeValue<int>("amount");
            FontMetrics::KernPair kernPair = std::make_pair(kFirst, kSecond);
            font->metrics.kern.insert_or_assign(kernPair, amount);
        }

        LoadedResource loadedFontResource(filePath, "font", info.id, SCAST<PJ::Base>(font));
        result.Add(loadedFontResource);

        return result;
    } catch (...) {
        PJLog("ERROR: Couldn't parse rTexPacker JSON");
    }

    return Failure();
}
