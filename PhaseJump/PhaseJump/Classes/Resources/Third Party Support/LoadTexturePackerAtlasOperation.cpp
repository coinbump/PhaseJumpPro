#include "LoadTexturePackerAtlasOperation.h"
#include "AtlasTexture.h"
#include "LoadResourcesModel.h"
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
        PJLog("ERROR. Can't open file at: %s", info.filePath.c_str());
        return Failure();
    }

    try {
        json j;
        file >> j;

        std::list<json::value_type> frames = j["frames"];

        FilePath fullImagePath = info.filePath;
        fullImagePath.replace_extension("png");

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
            auto atlasTexture = MAKE<AtlasTexture>(
                texture, atlasTextureId, atlasOrigin, atlasSize, trimOrigin, untrimmedSize,
                alphaMode
            );
            LoadedResource loadedResource(
                filePath, "texture", atlasTextureId, SCAST<PJ::Base>(atlasTexture)
            );
            result.Add(loadedResource);

            textureAtlas->Add(atlasTexture);
        }

        LoadedResource loadedAtlasResource(
            filePath, "texture.atlas", info.id, SCAST<PJ::Base>(textureAtlas)
        );
        result.Add(loadedAtlasResource);

        return result;
    } catch (...) {
        PJLog("ERROR: Couldn't parse rTexPacker JSON");
    }

    return Failure();
}
