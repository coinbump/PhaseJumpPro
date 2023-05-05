#ifndef PJGLTEXTURE_H
#define PJGLTEXTURE_H

#include "Base.h"
#include "RenderTypes.h"
#include "_Map.h"
#include "_String.h"
#include "GLHeaders.h"
#include "SomeTexture.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/21/23
 */
namespace PJ {
    class GLTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        GLTexture(String id, GLuint glId, Vector2Int size, TextureAlphaMode alphaMode)
        : Base(id, glId, size, alphaMode) {
        }

        virtual ~GLTexture() {
            if (renderId > 0) {
                glDeleteTextures(1, &renderId);
            }
        }

        void SetTextureMagnification(TextureMagnification value) override;
        void SetTextureWrap(TextureWrap value) override;
    };
}

#endif
