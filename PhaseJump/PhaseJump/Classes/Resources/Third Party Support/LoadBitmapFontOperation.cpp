#include "LoadBitmapFontOperation.h"
#include "AtlasTexture.h"
#include "CollectionUtils.h"
#include "Font.h"
#include "ResourceRepositoryModel.h"
#include "RTexPackerAtlasModelBuilder.h"
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

        // Get rid of these characters
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

            result.Set(first, second);
        } else if (IsNumericInt(second)) {
            int intValue = TypeValue<int>(second);
            result.Set(first, intValue);
        } else {
            result.Set(first, second);
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
        PJ::Log("ERROR. Can't open file at: ", info.filePath);
        return Failure();
    }

    Tags tags;

    try {
        Tags infoTags = ReadLine(file);
        Tags commonTags = ReadLine(file);
        Tags pageTags = ReadLine(file);
        Tags charsTags = ReadLine(file);

        VectorList<Tags> charLines;
        VectorList<Tags> kernLines;

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
        ResourceInfo loadTextureInfo{ .filePath = fullImagePath, .type = ResourceType::Texture };
        auto loadTextureOperations = repoModel.MakeLoadOperations(loadTextureInfo);

        GUARDR(!IsEmpty(loadTextureOperations), Failure())

        auto& loadTextureOperation = *(*loadTextureOperations.begin());

        loadTextureOperation.Run();
        GUARDR(loadTextureOperation.result, Failure())
        GUARDR(loadTextureOperation.result.value().IsSuccess(), Failure())

        auto successValue = loadTextureOperation.result.value().SuccessValue();

        GUARDR(successValue && successValue.value().size(), Failure())

        SP<SomeTexture> texture = DCAST<SomeTexture>(successValue.value().resources[0].resource);
        GUARDR(texture, Failure());

        Success result;

        auto textureAtlas = MAKE<TextureAtlas>(texture);

        auto font = MAKE<Font>(Font::Config{ .name = infoTags.SafeValue<String>("face"),
                                             .size = (float)infoTags.SafeValue<int>("size") });
        font->atlas = textureAtlas;

        for (auto& sprite : charLines) {
            // cout << sprite.ToString();

            String alphaMode = texture->alphaMode;
            Vector2Int size(sprite.SafeValue<int>("width"), sprite.SafeValue<int>("height"));
            Vector2Int pos(sprite.SafeValue<int>("x"), sprite.SafeValue<int>("y"));
            int id = sprite.SafeValue<int>("id");

            auto origin = pos;
            float normalOriginX = (float)origin.x / (float)texture->size.x;
            float normalOriginY = (float)origin.y / (float)texture->size.y;
            float normalSizeX = (float)size.x / (float)texture->size.x;
            float normalSizeY = (float)size.y / (float)texture->size.y;

            AtlasTexture::Config const& config{
                .base = { .id = MakeString(id),
                          .origin = origin,
                          .size = size,
                          .trimOrigin = Vector2Int::zero,
                          .untrimmedSize = size,
                          .normalOrigin = { normalOriginX, normalOriginY },
                          .normalSize = { normalSizeX, normalSizeY },
                          .alphaMode = alphaMode },
                .parent = textureAtlas.get()
            };
            auto atlasTexture = MAKE<AtlasTexture>(config);

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
            font->metrics.leading = commonTags.SafeValue<int>("lineHeight");
            font->metrics.descent = font->metrics.leading - font->metrics.ascent;
        }

        for (auto& kern : kernLines) {
            uint32_t kFirst = kern.SafeValue<int>("first");
            uint32_t kSecond = kern.SafeValue<int>("second");
            int amount = kern.SafeValue<int>("amount");
            FontMetrics::KernPair kernPair = std::make_pair(kFirst, kSecond);
            font->metrics.kern.insert_or_assign(kernPair, amount);
        }

        ResourceModel loadedFontResource{
            .resource = SCAST<PJ::Base>(font),
            .info = { .id = info.id, .filePath = filePath, .type = "font" }
        };
        result.Add(loadedFontResource);

        return result;
    } catch (...) {
        PJ::Log("ERROR: Couldn't parse rTexPacker JSON");
    }

    return Failure();
}
