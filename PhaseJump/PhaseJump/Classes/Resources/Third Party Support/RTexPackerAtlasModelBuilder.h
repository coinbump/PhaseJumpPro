#pragma once

#include "RTexPackerAtlasModel.h"
#include <json.hpp>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/20/24
 */
namespace PJ {
    /// Builds a model to parse a RTexPacker texture atlas file
    struct RTexPackerAtlasModelBuilder {
        /// Important: All JSON builders can throw an exception. Use with try/catch or not at all
        RTexPackerAtlasModel Build(nlohmann::json j);
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
