#pragma once

#include "Dev.h"
#include "GLHeaders.h"
#include "GLTexture.h"
#include "SomeRenderContext.h"
#include "Vector2.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 9/15/24
 */
namespace PJ {
    /// Wraps an OpenGL frame buffer and deletes it when it goes out of scope
    struct GLFrameBuffer {
        uint32_t id{};

        ~GLFrameBuffer();
    };

    /// Wraps an OpenGL render buffer and deletes it when it goes out of scope
    struct GLRenderBuffer {
        uint32_t id{};

        ~GLRenderBuffer();
    };

    /// Creates and manages an offscreen OpenGL texture buffer
    /// Useful when we need to render a viewport into the buffer and apply post processing effects
    /// via shaders.
    class GLTextureBuffer : public SomeRenderContext {
    public:
        using Base = SomeRenderContext;
        using This = GLTextureBuffer;

    protected:
        Vector2Int size;
        GLFrameBuffer frameBuffer;
        GLRenderBuffer depthBuffer;

        SP<GLTexture> texture;

        // FUTURE: Add stencil buffer support if needed
        // GLuint stencilBufferId = 0;

        // FUTURE: store memory cost of buffers for RAM tracking tools

    public:
        GLTextureBuffer(SP<SomeRenderEngine> renderEngine) :
            Base(renderEngine) {}

        virtual ~GLTextureBuffer();

        // MARK: SomeRenderContext

        SP<SomeTexture> Texture() const override {
            return texture;
        }

        void Build(Vector2Int size) override;
        void Bind() override;
        void Clear() override;

        /// A texture buffer can't present itself. Use the buffer texture to render it
        void Present() override {}

        Vector2 Size() const override {
            return Vector2(size.x, size.y);
        }

        Vector2Int PixelSize() const override {
            return size;
        }
    };
} // namespace PJ
