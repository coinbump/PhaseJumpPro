#pragma once

#include "GLHeaders.h"
#include "GLTexture.h"
#include "Log.h"
#include "SomeRenderContext.h"
#include "Vector2.h"

// TODO: Work in progress
// CODE REVIEW: /23?
namespace PJ {
    class GLTextureBuffer : public SomeRenderContext {
    protected:
        Vector2Int size;
        GLuint frameBufferId = 0;
        GLuint textureId = 0;
        GLuint depthBufferId = 0;

        SP<GLTexture> texture;

        // FUTURE: Add stencil buffer support if needed
        // GLuint stencilBufferId = 0;

        // FUTURE: store memory cost of buffers for RAM tracking tools

    public:
        virtual ~GLTextureBuffer();

        SP<SomeTexture> Texture() const {
            return texture;
        }

        // MARK: SomeRenderContext

        void Build(Vector2Int size) override;
        void Bind() override;
        void Clear() override;

        void Present() override {}

        Vector2 Size() const override {
            return Vector2(size.x, size.y);
        }

        Vector2Int PixelSize() const override {
            return size;
        }
    };
} // namespace PJ
