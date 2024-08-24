#pragma once

#include "RTexPackerAtlasModel.h"
#include <json.hpp>

namespace PJ {
    struct RTextPackerAtlasModelBuilder {
        /// Important: All JSON builders can throw an exception. Use with try/catch or not at all
        RTexPackerAtlasModel Build(nlohmann::json j) {
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
    };

    /// FUTURE: Investigate this approach and `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE` for JSON parsing
    /// (difficult with nested models)
    //    void from_json(nlohmann::json const& j, RTexPackerAtlasModel::Atlas& model) {
    //        j.at("imagePath").get_to(model.imagePath);
    //
    //        int width, height;
    //        j.at("width").get_to(width);
    //        j.at("height").get_to(height);
    //
    //        model.size = Vector2Int(width, height);
    //
    //        j.at("spriteCount").get_to(model.spriteCount);
    //        j.at("isFont").get_to(model.isFont);
    //        j.at("fontSize").get_to(model.fontSize);
    //    }
} // namespace PJ
