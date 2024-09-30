#pragma once

#include "Base.h"
#include "RenderTypes.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 8/8/24
 */
namespace PJ {
    struct TextureRenderModel {
        uint32_t renderId = 0;

        /// Normalized texture origin (0-1.0) in reading coordinates
        Vector2 origin;

        /// Normalized texture size (0-1.0)
        Vector2 size;

        TextureRenderModel(uint32_t renderId, Vector2 origin, Vector2 size) :
            renderId(renderId),
            origin(origin),
            size(size) {}
    };

    /// GPU texture that can be rendered in a Render Context
    class SomeTexture : public Base {
    protected:
        uint32_t renderId = 0;

    public:
        String id;
        Vector2Int size;

        String alphaMode = TextureAlphaMode::Standard;

        SomeTexture(String id, uint32_t renderId, Vector2Int size, String alphaMode) :
            id(id),
            renderId(renderId),
            size(size),
            alphaMode(alphaMode) {}

        virtual ~SomeTexture() {}

        Vector2Int Size() const {
            return size;
        }

        /// @return Returns the actual texture object used by the render engine
        /// In the case of a texture atlas, each child texture will have a link to its original
        /// render texture
        virtual SP<SomeTexture> RenderTexture() {
            return SCAST<SomeTexture>(shared_from_this());
        }

        virtual uint32_t RenderId() const {
            return renderId;
        }

        virtual void SetTextureMagnification(TextureMagnificationType value) {}

        virtual void SetTextureWrap(TextureWrapType value) {}

        /// Resolve texture to a render model we use for the render
        virtual std::optional<TextureRenderModel> MakeRenderModel() const {
            GUARDR(renderId, std::nullopt)
            return TextureRenderModel(renderId, vec2Zero, vec2One);
        }
    };
} // namespace PJ
