#include "GLTextureBuffer.h"
#include "Bitmap.h"
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
    GUARD(frameBuffer.id)

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    glRenderEngine->BindFrameBuffer(frameBuffer.id);
    glViewport(0, 0, size.x, size.y);
}

void GLTextureBuffer::Clear() {
    // FUTURE: support stencil buffer if needed
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLTextureBuffer::Build(Vector2Int size) {
    GUARD_LOG(!frameBuffer.id && !depthBuffer.id, "Can't rebuild existing buffer")

    auto glRenderEngine = static_cast<GLRenderEngine*>(&renderEngine);

    GLuint frameBufferId{};
    glGenFramebuffers(1, &frameBufferId);
    glRenderEngine->BindFrameBuffer(frameBufferId);
    renderId = frameBufferId;

    GLuint textureId{};
    glGenTextures(1, &textureId);

    // TODO: this is incorrect, we are using weird size and origin to "fix things"
    texture = MAKE<GLTexture>(GLTexture::Config{ .id = "buffer",
                                                 .renderId = textureId,
                                                 .size = size,
                                                 .normalOrigin = Vector2(0, 1),
                                                 .normalSize = Vector2(1, -1) });

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

    GUARD(CheckFrameBufferStatus())
    frameBuffer.id = frameBufferId;
    depthBuffer.id = depthBufferId;
    this->size = size;
}

bool GLTextureBuffer::IsValid() const {
    return frameBuffer.id != 0 && size.x > 0 && size.y > 0;
}
