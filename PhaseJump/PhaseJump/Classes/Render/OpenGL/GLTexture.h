#pragma once

#include "Base.h"
#include "GLHeaders.h"
#include "OrderedMap.h"
#include "RenderTypes.h"
#include "SomeTexture.h"
#include "StringUtils.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    class GLTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        GLTexture(Config const& config);

        virtual ~GLTexture();

        // MARK: SomeTexture

        void SetTextureMagnification(TextureMagnificationType value) override;
        void SetTextureWrap(TextureWrapType value) override;
    };

    GLint GLTextureMagnificationFor(TextureMagnificationType textureMagnification);
    GLint GLTextureWrapFor(TextureWrapType textureWrap);

} // namespace PJ
