#pragma once

#include "SomeFramePlayable.h"
#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "Utils.h"
#include <memory>

// CODE REVIEW: ?/23
namespace PJ {
    class RenderMaterial;
    struct Mesh;
    class TextureAtlas;

    /// Renders multiple textures as a sprite
    class AnimatedSpriteRenderer : public SomeRenderer {
    public:
        struct Frame {
            SP<SomeTexture> texture;
            Vector2 offset;

            Frame(SP<SomeTexture> texture, Vector2 offset) :
                texture(texture),
                offset(offset) {}
        };

        using Base = SomeRenderer;
        using TextureList = VectorList<SP<SomeTexture>>;

    protected:
        Mesh mesh;

        int frame = 0;
        float position = 0;

        VectorList<Frame> frames;

    public:
        bool flipX = false;
        bool flipY = false;
        SP<SomeFramePlayable> framePlayable;

        AnimatedSpriteRenderer(TextureList const& textures);
        AnimatedSpriteRenderer(TextureAtlas const& atlas);

        // MARK: SomeRenderer

        VectorList<RenderModel> MakeRenderModels(RenderIntoModel const& model) override;
        void SetColor(Color color) override;

        // MARK: WorldComponent

        void OnUpdate(TimeSlice time) override;
    };

    using AnimatedTextureRenderer = AnimatedSpriteRenderer;
} // namespace PJ
