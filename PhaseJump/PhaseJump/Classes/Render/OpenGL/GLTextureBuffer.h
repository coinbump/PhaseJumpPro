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
        // Don't allow copies
        DELETE_COPY(GLFrameBuffer);

        uint32_t id{};

        GLFrameBuffer() {}

        ~GLFrameBuffer();
    };

    /// Wraps an OpenGL render buffer and deletes it when it goes out of scope
    struct GLRenderBuffer {
        // Don't allow copies
        DELETE_COPY(GLRenderBuffer);

        uint32_t id{};

        GLRenderBuffer() {}

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
        GLTextureBuffer(SomeRenderEngine& renderEngine) :
            Base(renderEngine) {}

        virtual ~GLTextureBuffer();

        // MARK: SomeRenderContext

        SP<SomeTexture> Texture() const override {
            return texture;
        }

        // TODO: this is untested
        SP<Bitmap<PixelFormat::RGBA8888>> MakeBitmap() {
            GUARDR(IsValid(), nullptr);

            Data<uint32_t> data;
            data.ResizeCount(size.x * size.y);
            glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data.Pointer());

            return MAKE<Bitmap<PixelFormat::RGBA8888>>(Bitmap<PixelFormat::RGBA8888>::Config{
                .size = size, .data = &data });
        }

        bool IsValid() const;

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
