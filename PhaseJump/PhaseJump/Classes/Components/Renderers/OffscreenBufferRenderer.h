#pragma once

#include "SomeRenderer.h"
#include "SomeTexture.h"
#include <memory>

namespace PJ {
    /// Renders an offscreen buffer texture
    // TODO: is this just a textureRenderer? Or something differenet?
    //    class OffscreenBufferRenderer : public SomeRenderer {
    //    public:
    //        using Base = SomeRenderer;
    //        using This = OffscreenBufferRenderer;
    //
    //    protected:
    //        Color color = Color::white;
    //
    //        /// Material holds the render texture, this is the actual texture object (child
    //        texture for
    //        /// texture atlas)
    //        SP<SomeTexture> texture;
    //
    //    public:
    //        bool flipX = false;
    //        bool flipY = false;
    //
    //        // FUTURE: float pixelsPerUnit = 1.0f;
    //
    //        SpriteRenderer(SP<SomeTexture> texture);
    //        SpriteRenderer(SP<RenderMaterial> material);
    //
    //        Vector2 Size() const;
    //
    //        // MARK: SomeWorldComponent
    //
    //        String TypeName() const override {
    //            return "SpriteRenderer";
    //        }
    //
    //        // MARK: SomeRenderer
    //
    //        void SetColor(Color value) override;
    //
    //        VectorList<RenderModel> MakeRenderModels() override;
    //    };
    //
    //    using TextureRenderer = SpriteRenderer;
} // namespace PJ
