#pragma once

#include "Base.h"
#include "GLHeaders.h"
#include "OrderedMap.h"
#include "RenderTypes.h"
#include "StringUtils.h"
#include "Texture.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class GLTexture : public Texture {
    public:
        using Base = Texture;

        GLTexture(Config const& config);

        virtual ~GLTexture();

        // MARK: Texture

        void SetTextureMagnification(TextureMagnificationType value) override;
        void SetTextureWrap(TextureWrapType value) override;
    };

    GLint GLTextureMagnificationFor(TextureMagnificationType textureMagnification);
    GLint GLTextureWrapFor(TextureWrapType textureWrap);

} // namespace PJ
