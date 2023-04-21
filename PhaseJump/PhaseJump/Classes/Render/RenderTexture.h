#ifndef PJRENDERTEXTURE_H
#define PJRENDERTEXTURE_H

#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    /// Used to pass around a texture, carries id only, no reference counting or ownership
    struct RenderTexture {
        uint32_t id = 0;
        Vector2Int size;

        RenderTexture(uint32_t id, Vector2Int size) : id(id), size(size) {
        }
    };
}

#endif
