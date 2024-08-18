#pragma once

#include "Macros.h"
#include "SomeTexture.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 8/10/24
 */
namespace PJ {
    /// GPU texture that references only part of its parent texture (used for texture atlas, fonts)
    class AtlasTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        SP<SomeTexture> parent;

        /// Texture origin in reading coordinates (top-left is 0, 0)
        Vector2Int origin;

        /// Distance of trim from top-left of original image
        Vector2Int trimOrigin;

        /// Original texture size before trim
        Vector2Int trueSize;

        AtlasTexture(
            SP<SomeTexture> parent, String id, Vector2Int origin, Vector2Int size,
            Vector2Int trimOrigin, Vector2Int trueSize, String alphaMode
        ) :
            Base(id, 0, size, alphaMode),
            parent(parent),
            origin(origin),
            trimOrigin(trimOrigin),
            trueSize(trueSize) {}

        std::optional<TextureRenderModel> MakeRenderModel() const override {
            GUARDR(parent, Base::MakeRenderModel())
            GUARDR(parent->size.x > 0 && parent->size.y > 0, Base::MakeRenderModel())

            float normalOriginX = (float)origin.x / (float)parent->size.x;
            float normalOriginY = (float)origin.y / (float)parent->size.y;
            Vector2 normalOrigin(normalOriginX, normalOriginY);

            float normalSizeX = (float)size.x / (float)parent->size.x;
            float normalSizeY = (float)size.y / (float)parent->size.y;
            Vector2 normalSize(normalSizeX, normalSizeY);

            return TextureRenderModel(RenderId(), normalOrigin, normalSize);
        }

        // MARK: SomeTexture

        SP<SomeTexture> RenderTexture() override {
            GUARDR(parent, Base::RenderTexture())
            return parent;
        }

        uint32_t RenderId() const override {
            GUARDR(parent, 0)
            return parent->RenderId();
        }
    };
} // namespace PJ
