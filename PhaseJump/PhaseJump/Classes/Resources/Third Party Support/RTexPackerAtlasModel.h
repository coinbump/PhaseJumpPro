#pragma once

#include "_String.h"
#include "List.h"
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
            int spriteCount = 0;
            bool isFont = false;
            int fontSize = 0;
        };

        struct Char {
            uint32_t value;
            Vector2Int offset;
            int advanceX;
        };

        struct Sprite {
            String nameId;
            Vector2Int position;
            Vector2Int sourceSize;
            bool isTrimmed = false;
            Vector2Int trimOrigin;
            Vector2Int trimSize;
            std::optional<Char> _char;
        };

        Atlas atlas;
        List<Sprite> sprites;
    };
} // namespace PJ
