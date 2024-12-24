#include "LoadTexturePackerAtlasOperation.h"
#include "AtlasTexture.h"
#include "ResourceRepositoryModel.h"
#include "SDLLoadGLTextureOperation.h"
#include "StringUtils.h"
#include "TextureAtlas.h"
#include "Utils.h"
#include <json.hpp>

using namespace std;
using namespace PJ;
using namespace nlohmann;

SomeLoadResourcesOperation::Result LoadTexturePackerAtlasOperation::LoadResources() {
    FilePath filePath = info.filePath;

    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        PJ::Log("ERROR. Can't open file at: ", info.filePath);
        return Failure();
    }

    try {
        json j;
        file >> j;

        std::list<json::value_type> frames = j["frames"];

        FilePath fullImagePath = info.filePath;
        fullImagePath.replace_extension("png");

        // No id needed, we're not storing the parent texture in resources
        ResourceInfo loadTextureInfo{ .filePath = fullImagePath, .type = ResourceType::Texture };
        auto loadTextureOperations = repoModel.MakeLoadOperations(loadTextureInfo);

        GUARDR(!IsEmpty(loadTextureOperations), Failure())

        auto& loadTextureOperation = *(*loadTextureOperations.begin());

        loadTextureOperation.Run();
        GUARDR(loadTextureOperation.result, Failure())
        GUARDR(loadTextureOperation.result->IsSuccess(), Failure())
        auto successValue = loadTextureOperation.result->SuccessValue();

        GUARDR(successValue && successValue->size() > 0, Failure())

        SP<SomeTexture> texture = DCAST<SomeTexture>(successValue->resources[0].resource);
        GUARDR(texture, Failure());

        Success result;

        auto textureAtlas = MAKE<TextureAtlas>(texture);
        GUARDR(texture->size.x > 0 && texture->size.y > 0, Failure());

        for (auto& frame : frames) {
            String fileName = frame["filename"];
            auto fileNameComponents = ComponentsSeparatedBy(fileName, '.');
            String atlasTextureId = IsEmpty(fileNameComponents) ? fileName : fileNameComponents[0];

            auto frameInfo = frame["frame"];

            int positionX = frameInfo["x"];
            int positionY = frameInfo["y"];

            auto sourceSize = frame["sourceSize"];
            int sourceWidth = sourceSize["w"];
            int sourceHeight = sourceSize["h"];

            bool isRotated = frame["rotated"];

            auto spriteSourceSize = frame["spriteSourceSize"];
            int trimRecX = spriteSourceSize["x"];
            int trimRecY = spriteSourceSize["y"];

            int trimWidth = spriteSourceSize["w"];
            int trimHeight = spriteSourceSize["h"];

            Vector2Int atlasOrigin(positionX, positionY);
            Vector2Int atlasSize(trimWidth, trimHeight);
            Vector2Int trimOrigin(trimRecX, trimRecY);
            Vector2Int untrimmedSize(sourceWidth, sourceHeight);
            String alphaMode = texture->alphaMode;

            auto origin = atlasOrigin;
            auto size = atlasSize;
            float normalOriginX = (float)origin.x / (float)texture->size.x;
            float normalOriginY = (float)origin.y / (float)texture->size.y;
            float normalSizeX = (float)size.x / (float)texture->size.x;
            float normalSizeY = (float)size.y / (float)texture->size.y;

            AtlasTexture::Orientation orientation =
                isRotated ? AtlasTexture::Orientation::Rotated : AtlasTexture::Orientation::Default;
            // TODO: add rotated support (not yet supported)
            //            switch (orientation) {
            //            case AtlasTexture::Orientation::Default:
            //                break;
            //            case AtlasTexture::Orientation::Rotated:
            //                // Texture packer stores rotated textures with the unrotated size and
            //                frame, so
            //                // adjust
            //                normalSizeX = (float)size.x / (float)texture->size.y;
            //                normalSizeY = (float)size.y / (float)texture->size.x;
            //                //                std::swap(size.x, size.y);
            //                //                std::swap(untrimmedSize.x, untrimmedSize.y);
            //                break;
            //            }

            AtlasTexture::Config config{ .base = { .id = atlasTextureId,
                                                   .origin = origin,
                                                   .size = size,
                                                   .trimOrigin = trimOrigin,
                                                   .untrimmedSize = untrimmedSize,
                                                   .alphaMode = alphaMode,
                                                   .normalOrigin = { normalOriginX, normalOriginY },
                                                   .normalSize = { normalSizeX, normalSizeY } },
                                         .parent = textureAtlas.get(),
                                         .orientation = orientation };
            auto atlasTexture = MAKE<AtlasTexture>(config);

            ResourceModel loadedResource{ .resource = SCAST<PJ::Base>(atlasTexture),
                                          .info = { .id = atlasTextureId,
                                                    .filePath = filePath,
                                                    .type = ResourceType::Texture } };
            result.Add(loadedResource);

            textureAtlas->Add(atlasTexture);
        }

        ResourceModel loadedAtlasResource{
            .resource = SCAST<PJ::Base>(textureAtlas),
            .info = { .id = info.id, .filePath = filePath, .type = "texture.atlas" }
        };
        result.Add(loadedAtlasResource);

        return result;
    } catch (...) {
        PJ::Log("ERROR: Couldn't parse rTexPacker JSON");
    }

    return Failure();
}
