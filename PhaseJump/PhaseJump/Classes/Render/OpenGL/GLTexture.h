#ifndef PJGLTEXTURE_H
#define PJGLTEXTURE_H

#include "_String.h"
#include "Base.h"
#include "GLHeaders.h"
#include "OrderedMap.h"
#include "RenderTypes.h"
#include "SomeTexture.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class GLTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        GLTexture(String id, GLuint glId, Vector2Int size, String alphaMode) :
            Base(id, glId, size, alphaMode) {}

        virtual ~GLTexture() {
            if (renderId > 0) {
                glDeleteTextures(1, &renderId);
            }
        }

        static GLint GLTextureMagnification(TextureMagnificationType textureMagnification) {
            OrderedMap<TextureMagnificationType, GLint> map = {
                { TextureMagnification::Nearest, GL_NEAREST },
                { TextureMagnification::Linear, GL_LINEAR },
            };
            auto i = map.find(textureMagnification);
            GUARDR(i != map.end(), GL_LINEAR);
            return i->second;
        }

        static GLint GLTextureWrap(TextureWrapType textureWrap) {
            OrderedMap<TextureWrapType, GLint> map = {
                { TextureWrap::Clamp, GL_CLAMP_TO_EDGE },
                { TextureWrap::Repeat, GL_REPEAT },
            };
            auto i = map.find(textureWrap);
            GUARDR(i != map.end(), GL_CLAMP_TO_EDGE);
            return i->second;
        }

        // MARK: SomeTexture

        void SetTextureMagnification(TextureMagnificationType value) override;
        void SetTextureWrap(TextureWrapType value) override;
    };
} // namespace PJ

#endif
