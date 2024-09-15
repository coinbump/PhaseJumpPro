#pragma once

#include "SomeFramePlayable.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "Utils.h"
#include <memory>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/6/24
 */
namespace PJ {
    class RenderMaterial;
    struct Mesh;
    class TextureAtlas;

    /// Renders multiple textures as a sprite
    class AnimatedSpriteRenderer : public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using This = AnimatedSpriteRenderer;
        using TextureList = VectorList<SP<SomeTexture>>;

        struct Frame {
            SP<SomeTexture> texture;
            Vector2 offset;

            Frame(SP<SomeTexture> texture, Vector2 offset) :
                texture(texture),
                offset(offset) {}
        };

    protected:
        int frame = 0;
        float position = 0;

        VectorList<Frame> frames;

    public:
        bool flipX = false;
        bool flipY = false;
        SP<SomeFramePlayable> framePlayable;

        AnimatedSpriteRenderer(TextureList const& textures);
        AnimatedSpriteRenderer(TextureAtlas const& atlas);

        Vector2 Size() const;

        // MARK: SomeWorldComponent

        String TypeName() const override {
            return "AnimatedSpriteRenderer";
        }

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels() override;

        // MARK: WorldComponent

        void OnUpdate(TimeSlice time) override;
    };

    using AnimatedTextureRenderer = AnimatedSpriteRenderer;
} // namespace PJ
