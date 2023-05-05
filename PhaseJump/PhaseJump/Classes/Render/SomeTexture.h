#ifndef PJSOMETEXTURE_H
#define PJSOMETEXTURE_H

#include "Base.h"
#include "Vector2.h"
#include "RenderTypes.h"

/*
 RATING: 5 stars
 Simple class
 CODE REVIEW: 5/1/23
 */
namespace PJ {
    /// <summary>
    /// A GPU texture that can be rendered in a Render Context
    /// </summary>
    class SomeTexture : public Base {
    public:
        String id;
        uint32_t renderId = 0;
        Vector2Int size;

        TextureAlphaMode alphaMode = TextureAlphaMode::Standard;

        SomeTexture(String id, uint32_t renderId, Vector2Int size, TextureAlphaMode alphaMode) :
        id(id),
        renderId(renderId),
        size(size),
        alphaMode(alphaMode) {
        }

        virtual ~SomeTexture() { }

        virtual void SetTextureMagnification(TextureMagnification value) {}
        virtual void SetTextureWrap(TextureWrap value) {}
    };
}

#endif
