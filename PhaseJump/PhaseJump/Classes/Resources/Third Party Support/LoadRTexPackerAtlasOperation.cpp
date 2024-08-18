#include "LoadRTexPackerAtlasOperation.h"
#include "AtlasTexture.h"
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

        // FUTURE: this should be created with a factory so it's not constrained to OpenGL
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

        for (auto& sprite : model.sprites) {
            String alphaMode = texture->alphaMode;
            auto atlasTexture = MAKE<AtlasTexture>(
                texture, sprite.nameId, sprite.position, sprite.trimSize, sprite.trimOrigin,
                sprite.sourceSize, alphaMode
            );

            LoadedResource loadedResource(
                filePath, "texture", sprite.nameId, SCAST<PJ::Base>(atlasTexture)
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
