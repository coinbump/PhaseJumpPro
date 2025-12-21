#include "SomeRenderContext.h"
#include <stdio.h>

// Experimental
namespace PJ {
    class SomeTextureBuffer : public RenderContext {};
} // namespace PJ

#if FALSE

class GLTextureBuffer : public RenderContext, public Drawable {
public:
    GLuint glBufferTextureId = 0;
    GLuint glFrameBufferId = 0;
    Vector2Int size;
    Color clearColor;
    bool mDepth;

    virtual void Build(Vector2Int size) = 0;

    GLTextureBuffer();
    virtual ~GLTextureBuffer();

    GLuint GetTextureID() const {
        return glBufferTextureId;
    }

    void PreRenderOrtho(bool clear);

    // OVERRIDE:
    virtual void Present() {} // Use RenderAt2D

    virtual Vector2 Size() const {
        return Vector2(static_cast<float>(size.x()), static_cast<float>(size.y()));
    }

    virtual uint32_t PixelWidth() const {
        return size.x();
    }

    virtual uint32_t PixelHeight() const {
        return size.y();
    }
};

GLTextureBuffer::GLTextureBuffer() {
    mRenderMode = RenderMode(kRenderModeTextureCopy, RenderBlend::BlendNone);

    glBufferTextureId = 0;
    glFrameBufferId = 0;
    clearColor = ColorToFColor(Color::white);
}

GLTextureBuffer::~GLTextureBuffer() {

    delete mDevRAMToken;
}

void GLTextureBuffer::PreRenderOrtho(bool clear) {
    Bind();

    // Set the viewport size.
    renderEngine->SetViewport(0, 0, size.x(), size.y());

    renderEngine->SetActiveMatrixStack(MatrixStack::REProjectionStack);
    // FUTURE: evaluate near, far values for orthographic projection.
    // Currently z axis points towards us.
    renderEngine->LoadOrtho(
        0, static_cast<float>(size.x()), static_cast<float>(size.y()), 0, 0.0, 1.0f
    );
    renderEngine->SetActiveMatrixStack(MatrixStack::REModelViewStack);
    renderEngine->LoadIdentity();

    // Clear the background.
    /*
        PLATFORM iOS NOTE: 2/11/2013

        When rendering to a texure buffer, screen tearing can occur if glClear
       is called and a UIView is opened over the OpenGL view. The cause is
       uncertain.

        BEST PRACTICE: when opening a UIView, create a temporary opaque layer
       that blocks renders of OpenGL views using offscreen texture buffers, to
       prevent screen tears.

     */
    if (clear) {
        glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
        Clear();
    }
}

GLTextureBuffer::GLTextureBuffer() {
    mRenderMode = RenderMode(kRenderModeTextureCopy, RenderBlend::BlendNone);

    glBufferTextureId = 0;
    glFrameBufferId = 0;
    clearColor = ColorToFColor(Color::white);
}

GLTextureBuffer::~GLTextureBuffer() {}

void GLTextureBuffer::PreRenderOrtho(bool clear) {
    Bind();

    // Set the viewport size.
    renderEngine->SetViewport(0, 0, size.x(), size.y());

    renderEngine->SetActiveMatrixStack(MatrixStack::REProjectionStack);
    // FUTURE: evaluate near, far values for orthographic projection.
    // Currently z axis points towards us.
    renderEngine->LoadOrtho(
        0, static_cast<float>(size.x()), static_cast<float>(size.y()), 0, 0.0, 1.0f
    );
    renderEngine->SetActiveMatrixStack(MatrixStack::REModelViewStack);
    renderEngine->LoadIdentity();

    // Clear the background.
    /*
        PLATFORM iOS NOTE: 2/11/2013

        When rendering to a texure buffer, screen tearing can occur if glClear
       is called and a UIView is opened over the OpenGL view. The cause is
       uncertain.

        BEST PRACTICE: when opening a UIView, create a temporary opaque layer
       that blocks renders of OpenGL views using offscreen texture buffers, to
       prevent screen tears.

     */
    if (clear) {
        glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
        Clear();
    }
}

PJ_GLES2TextureBuffer::PJ_GLES2TextureBuffer() {}

PJ_GLES2TextureBuffer::~PJ_GLES2TextureBuffer() {
    PJ::renderState.InvalidateFrameBuffer(mFrameBuffer); // This frame buffer is no longer bound.
    PJ::renderState.InvalidateTexture(mBufferTexture);   // This texture is no longer bound.

    if (mFrameBuffer != 0) {
        glDeleteFramebuffers(1, &mFrameBuffer);
        mFrameBuffer = 0;
    }
    if (mBufferTexture != 0) {
        glDeleteTextures(1, &mBufferTexture);
        mBufferTexture = 0;
    }
    if (mDepthStencilBuffer != 0) {
        glDeleteRenderbuffers(1, &mDepthStencilBuffer);
    }
}

void PJ_GLES2TextureBuffer::Bind() {
    renderEngine->BindFrameBuffer(mFrameBuffer);
}

void PJ_GLES2TextureBuffer::Clear() {
    // MISSING: send to render engine
    glClear(GL_COLOR_BUFFER_BIT);
}

void PJ_GLES2TextureBuffer::Build(PJ_Vector2Int size) {
    renderEngine->PushRenderState(); // Might be in the middle of a render.

    mSize = size;
    mBounds.SetSize(
        static_cast<float>(mSize.x()), static_cast<float>(mSize.y())
    ); // For drawable size queries.

    // Create and bind the frame buffer.
    glGenFramebuffers(1, &mFrameBuffer);
    // PJ::Log("Old Buffer: %d, Texture buffer: %d", oldBuffer, mFrameBuffer);
    renderEngine->BindFrameBuffer(mFrameBuffer);

    // Create the texture, set up parameters.
    glGenTextures(1, &mBufferTexture);
    renderEngine->BindTexture2D(mBufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // IMPORTANT: without these parameters set, the texture will render as a
    // black image.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // ? GL_RGBA8 ?
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, mSize.x(), mSize.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
    );
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBufferTexture, 0);

    // Stencil buffer: for clipping (don't use glScissor).
    // https://www.khronos.org/registry/gles/extensions/OES/OES_packed_depth_stencil.txt
    glGenRenderbuffers(1, &mDepthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
#ifdef GL_DEPTH24_STENCIL8_OES
        GL_DEPTH24_STENCIL8_OES,
#else
        GL_STENCIL_INDEX8,
#endif
        size.x(), size.y()
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilBuffer
    );

#ifdef __DEBUG__
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PJ::Log(
            "Failed to make complete texture buffer object ",
            glCheckFramebufferStatus(GL_FRAMEBUFFER)
        );
    }
#endif

    renderEngine->PopRenderState();
}

void PJ_GLES2TextureBuffer::RenderAt2D(PJ_REQuad const& v) {
    renderEngine->SetRenderMode(mRenderMode);

    // NOTE: in OpenGL, textures are stored "upside down", so the texture
    // coordinates for rendering the texture buffer must be adjusted
    // accordingly.
    REfloat texCoords[8] = { 0, 1.0, 1.0, 1.0, 0, 0.0, 1.0, 0.0 };
    renderEngine->TexCoordPointer(2, GL_FLOAT, 0, texCoords);
    renderEngine->BindTexture2D(mBufferTexture); // FUTURE: BindTexture2D is correct naming
    renderEngine->Render2DQuad(PJ_REQuad(v));    // FUTURE: optimize with const-correctness
}

#endif
