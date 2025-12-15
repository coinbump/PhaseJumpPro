#pragma once

#include "List.h"
#include "StringUtils.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/11/24
 */
namespace PJ {
    /// Model defined by the rTexPacker JSON file format
    struct RTexPackerAtlasModel {
        struct Atlas {
            String imagePath;
            Vector2Int size;
            int spriteCount{};
            bool isFont{};
            int fontSize{};
        };

        struct Char {
            uint32_t value{};
            Vector2Int offset;
            int advanceX{};
        };

        struct Sprite {
            String nameId;
            Vector2Int position;
            Vector2Int sourceSize;
            bool isTrimmed{};
            Vector2Int trimOrigin;
            Vector2Int trimSize;
            std::optional<Char> _char;
        };

        Atlas atlas;
        VectorList<Sprite> sprites;
    };
} // namespace PJ
