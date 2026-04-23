#pragma once

#include "Bitmap.h"
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
    public:
        // Don't allow copies
        DELETE_COPY(GLFrameBuffer);

    protected:
        RenderItemId id{};

    public:
        RenderItemId Id() const {
            return id;
        }

        GLFrameBuffer() {}

        GLFrameBuffer(RenderItemId id) :
            id(id) {}

        ~GLFrameBuffer();

        void Delete() {
            GUARD(id)
            glDeleteFramebuffers(1, &id);
            id = 0;
        }
    };

    /// Wraps an OpenGL render buffer and deletes it when it goes out of scope
    struct GLRenderBuffer {
    public:
        // Don't allow copies
        DELETE_COPY(GLRenderBuffer);

    protected:
        RenderItemId id{};

    public:
        RenderItemId Id() const {
            return id;
        }

        GLRenderBuffer() {}

        GLRenderBuffer(RenderItemId id) :
            id(id) {}

        ~GLRenderBuffer();

        void Delete() {
            GUARD(id)
            glDeleteRenderbuffers(1, &id);
            id = 0;
        }
    };

    // TODO: fix resource leak bugs for early exit
    /// Creates and manages an offscreen OpenGL texture buffer
    /// Useful when we need to render a viewport into the buffer and apply post processing effects
    /// via shaders.
    class GLTextureBuffer : public RenderContext {
    public:
        using Base = RenderContext;
        using This = GLTextureBuffer;

    protected:
        Vector2Int size;
        UP<GLFrameBuffer> frameBuffer;
        UP<GLRenderBuffer> depthBuffer;

        SP<GLTexture> texture;

        // FUTURE: Add stencil buffer support if needed
        // GLuint stencilBufferId = 0;

        // FUTURE: store memory cost of buffers for RAM tracking tools

    public:
        GLTextureBuffer(SomeRenderEngine& renderEngine) :
            Base(renderEngine) {}

        virtual ~GLTextureBuffer();

        // MARK: SomeRenderContext

        SP<Texture> GetTexture() const override {
            return texture;
        }

        RenderItemId RenderId() const override {
            return texture ? texture->RenderId() : 0;
        }

        /// Reads the buffer's pixels into a new standard-oriented RGBA bitmap.
        /// Binds this buffer for the read and restores the previously bound framebuffer.
        UP<Bitmap<PixelFormat::RGBA8888>> NewBitmap();

        bool IsValid() const;

        void Build(Vector2Int size) override;
        void Resize(Vector2Int size) override;
        void Bind() override;
        void Clear() override;
        void Swap(SP<Texture> texture) override;

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
