#include "RTexPackerAtlasModelBuilder.h"
#include "FilePath.h"

using namespace std;
using namespace PJ;

RTexPackerAtlasModel RTexPackerAtlasModelBuilder::Build(nlohmann::json j) {
    RTexPackerAtlasModel result;

    auto atlas = j["atlas"];

    result.atlas.imagePath = atlas["imagePath"];
    result.atlas.size = Vector2Int(atlas["width"], atlas["height"]);
    result.atlas.spriteCount = atlas["spriteCount"];
    result.atlas.isFont = atlas["isFont"];
    result.atlas.fontSize = atlas["fontSize"];

    FilePath imagePath = atlas["imagePath"];
    std::list<nlohmann::json::value_type> sprites = j["sprites"];

    for (auto& sprite : sprites) {
        RTexPackerAtlasModel::Sprite resultSprite;

        resultSprite.nameId = sprite["nameId"];

        auto position = sprite["position"];
        resultSprite.position = Vector2Int(position["x"], position["y"]);

        auto sourceSize = sprite["sourceSize"];
        resultSprite.sourceSize = Vector2Int(sourceSize["width"], sourceSize["height"]);

        auto trimRec = sprite["trimRec"];
        resultSprite.trimOrigin = Vector2Int(trimRec["x"], trimRec["y"]);
        resultSprite.trimSize = Vector2Int(trimRec["width"], trimRec["height"]);

        // For packed font atlases
        if (sprite.contains("char")) {
            nlohmann::json::value_type spriteChar = sprite["char"];
            int charValue = spriteChar["value"];
            int offsetX = spriteChar["offset"]["x"];
            int offsetY = spriteChar["offset"]["y"];

            RTexPackerAtlasModel::Char _char;
            _char.value = charValue;
            _char.offset = Vector2Int(offsetX, offsetY);
            _char.advanceX = spriteChar["advanceX"];

            resultSprite._char = _char;
        }

        result.sprites.push_back(resultSprite);
    }

    return result;
}
