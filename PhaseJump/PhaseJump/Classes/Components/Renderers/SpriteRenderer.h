#pragma once

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    /// Renders a single texture as a sprite
    class SpriteRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = SpriteRenderer;

    protected:
        /// Material holds the render texture, this is the actual texture object (child texture for
        /// texture atlas)
        SP<SomeTexture> texture;

        void Configure();

    public:
        bool flipX = false;
        bool flipY = false;

        // FUTURE: float pixelsPerUnit = 1.0f;

        SpriteRenderer(SP<SomeTexture> texture);
        SpriteRenderer(SP<RenderMaterial> material);

        Vector2 Size() const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SpriteRenderer";
        }
    };

    using TextureRenderer = SpriteRenderer;
} // namespace PJ
