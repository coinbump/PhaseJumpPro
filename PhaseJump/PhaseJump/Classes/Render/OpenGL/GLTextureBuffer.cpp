#include "GLTextureBuffer.h"
#include "Bitmap.h"
#include "BitmapOperations.h"
#include "GLBufferBindingScope.h"
#include "GLRenderEngine.h"
#include "GLTexture.h"

using namespace std;
using namespace PJ;

GLFrameBuffer::~GLFrameBuffer() {
    if (id) {
        glDeleteFramebuffers(1, &id);
    }
}

GLRenderBuffer::~GLRenderBuffer() {
    if (id) {
        glDeleteRenderbuffers(1, &id);
    }
}

GLTextureBuffer::~GLTextureBuffer() {}

bool CheckFrameBufferStatus() {
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        PJ::Log("GL Texture buffer format unsupported ", status);
        break;
    default:
        PJ::Log("Failed to build GL texture buffer ", status);
        break;
    }
    return false;
}

void GLTextureBuffer::Bind() {
    GUARD(frameBuffer && frameBuffer->Id())

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    glRenderEngine->BindFrameBuffer(frameBuffer->Id());
    glViewport(0, 0, size.x, size.y);
}

void GLTextureBuffer::Clear() {
    GUARD(clearType != RenderClearType::None)

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    // Set the value to clear the stencil buffer to
    glClearStencil(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLTextureBuffer::Resize(Vector2Int size) {
    GUARD_LOG(size.x > 0 && size.y > 0, "Can't resize to non-positive size")
    GUARD(size != this->size)

    frameBuffer.reset();
    depthBuffer.reset();
    texture.reset();

    renderId = 0;
    this->size = {};

    Build(size);
}

void GLTextureBuffer::Build(Vector2Int size) {
    GUARD_LOG(!frameBuffer && !depthBuffer, "Can't rebuild existing buffer")

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    GLuint frameBufferId{};
    glGenFramebuffers(1, &frameBufferId);
    glRenderEngine->BindFrameBuffer(frameBufferId);
    renderId = frameBufferId;

    auto pendingFrameBuffer = NEW<GLFrameBuffer>(frameBufferId);

    GLuint textureId{};
    glGenTextures(1, &textureId);

    texture = MAKE<GLTexture>(GLTexture::Config{ .id = "buffer",
                                                 .renderId = textureId,
                                                 .size = size,
                                                 .normalOrigin = Vector2(0, 0),
                                                 .normalSize = Vector2(1, 1) });

    glRenderEngine->BindTexture2D(textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // FUTURE: evaluate advantages of using GL_DRAW_FRAMEBUFFER here
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    GLuint depthBufferId{};
    glGenRenderbuffers(1, &depthBufferId);
    glRenderEngine->BindRenderBuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);

    // FUTURE: evaluate advantages of using GL_DRAW_FRAMEBUFFER here
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);

    auto pendingDepthBuffer = NEW<GLRenderBuffer>(depthBufferId);

    GUARD(CheckFrameBufferStatus())
    frameBuffer = std::move(pendingFrameBuffer);
    depthBuffer = std::move(pendingDepthBuffer);
    this->size = size;
}

void GLTextureBuffer::Swap(SP<Texture> newTexture) {
    GUARD(newTexture)
    auto glTexture = DCAST<GLTexture>(newTexture);
    GUARD_LOG(glTexture, "GLTextureBuffer::Swap requires a GLTexture")
    GUARD(glTexture->RenderId())
    GUARD(frameBuffer && frameBuffer->Id())

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    // Capture the current framebuffer/renderbuffer bindings; restored on scope exit. Swap may
    // run mid-frame, and leaving our framebuffer bound would silently redirect subsequent
    // draws until the next Bind.
    GLBufferBindingScope bindingScope(*glRenderEngine);

    glRenderEngine->BindFrameBuffer(frameBuffer->Id());

    // Re-point the framebuffer's color attachment at the incoming texture. The previously
    // attached texture is released when we reassign `this->texture` below.
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture->RenderId(), 0
    );

    auto newSize = glTexture->Size();

    // Resize the depth buffer so it matches the new color attachment's dimensions.
    if (depthBuffer && depthBuffer->Id()) {
        glRenderEngine->BindRenderBuffer(GL_RENDERBUFFER, depthBuffer->Id());
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, newSize.x, newSize.y);
    }

    texture = glTexture;
    size = newSize;

    (void)CheckFrameBufferStatus();
}

bool GLTextureBuffer::IsValid() const {
    GUARDR(frameBuffer, false)

    return frameBuffer->Id() != 0 && size.x > 0 && size.y > 0;
}

UP<Bitmap<PixelFormat::RGBA8888>> GLTextureBuffer::NewBitmap() {
    GUARDR(IsValid(), nullptr);

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    GLBufferBindingScope bindingScope(*glRenderEngine);

    Bind();

    auto bitmap =
        NEW<Bitmap<PixelFormat::RGBA8888>>(Bitmap<PixelFormat::RGBA8888>::Config{ .size = size });
    glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->Pixels().data());

    BitmapOperations::NewFlipVertical(UpdateOrientationType::None)->Run(*bitmap);

    return bitmap;
}
