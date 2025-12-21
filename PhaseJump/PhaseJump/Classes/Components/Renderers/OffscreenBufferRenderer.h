#pragma once

#include "Renderer.h"
#include "Texture.h"
#include <memory>

// Unused. Future feature?
namespace PJ {
    /// Renders an offscreen buffer texture
    // is this just a textureRenderer? Or something different?
    //    class OffscreenBufferRenderer : public SomeMaterialRenderer {
    //    public:
    //        using Base = SomeMaterialRenderer;
    //        using This = OffscreenBufferRenderer;
    //
    //    protected:
    //        Color color = Color::white;
    //
    //        /// Material holds the render texture, this is the actual texture object (child
    //        texture for
    //        /// texture atlas)
    //        SP<Texture> texture;
    //
    //    public:
    //        bool flipX = false;
    //        bool flipY = false;
    //
    //        // FUTURE: float pixelsPerUnit = 1.0f;
    //
    //        SpriteRenderer(SP<Texture> texture);
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
    //        // MARK: Renderer
    //
    //        void SetColor(Color value) override;
    //
    //        VectorList<RenderModel> MakeRenderModels() override;
    //    };
    //
    //    using TextureRenderer = SpriteRenderer;
} // namespace PJ
