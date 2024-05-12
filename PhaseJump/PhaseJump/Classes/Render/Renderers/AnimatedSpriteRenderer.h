#pragma once

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include "SomeFrameAnimator.h"
#include "Macros.h"
#include <memory>

namespace PJ {
    class RenderMaterial;
    struct Mesh;

    /// Renders multiple textures as a sprite
    class AnimatedSpriteRenderer: public SomeRenderer {
    public:
        using Base = SomeRenderer;
        using TextureList = VectorList<SP<SomeTexture>>;

    protected:
        Mesh mesh;

        int frame = 0;
        float position = 0;

        TextureList textures;

    public:
        bool flipX = false;
        bool flipY = false;
        SP<SomeFrameAnimator> frameAnimator;

        SP<RenderMaterial> material;

        AnimatedSpriteRenderer(TextureList const& textures);

        void RenderInto(RenderIntoModel model) override;
        void SetColor(Color color) override;
        void OnUpdate(TimeSlice time) override;
    };
}
