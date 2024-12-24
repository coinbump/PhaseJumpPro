#pragma once

#include "RenderFeature.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/22/24
 */
namespace PJ {
    /// Renders a single texture as a sprite
    class SpriteRenderer : public SomeMaterialRenderer {
    public:
        using Base = SomeMaterialRenderer;
        using This = SpriteRenderer;

    protected:
        bool flipX = false;
        bool flipY = false;

        void Configure();

    public:
        // FUTURE: float pixelsPerUnit = 1.0f;

        struct Config {
            using FeatureStateMap = RenderMaterial::FeatureStateMap;

            SP<SomeTexture> texture;
            SP<RenderMaterial> material;

            FeatureStateMap features = { { RenderFeature::Blend, RenderFeatureState::Enable } };
        };

        SpriteRenderer(Config config);
        SpriteRenderer(SP<SomeTexture> texture);
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
