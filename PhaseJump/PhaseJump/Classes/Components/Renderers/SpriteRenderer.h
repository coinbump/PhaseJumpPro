#pragma once

#include "Renderer.h"
#include "RenderFeature.h"
#include "Texture.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Renders a single texture as a sprite
    class SpriteRenderer : public MaterialRenderer {
    public:
        using Base = MaterialRenderer;
        using This = SpriteRenderer;

    protected:
        bool flipX = false;
        bool flipY = false;

        void Configure();

    public:
        // FUTURE: float pixelsPerUnit = 1.0f;

        struct Config {
            using FeatureStateMap = RenderMaterial::FeatureStateMap;

            SP<Texture> texture;
            SP<RenderMaterial> material;

            FeatureStateMap features = { { RenderFeature::Blend, RenderFeatureState::Enable } };
        };

        SpriteRenderer(Config const& config);
        SpriteRenderer(SP<Texture> texture);
        SpriteRenderer(SP<RenderMaterial> material);

        void SetFlipX(bool value) {
            GUARD(flipX != value)
            flipX = value;
            model.SetMeshNeedsBuild();
        }

        void SetFlipY(bool value) {
            GUARD(flipY != value)
            flipY = value;
            model.SetMeshNeedsBuild();
        }

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "SpriteRenderer";
        }
    };

    using TextureRenderer = SpriteRenderer;
} // namespace PJ
