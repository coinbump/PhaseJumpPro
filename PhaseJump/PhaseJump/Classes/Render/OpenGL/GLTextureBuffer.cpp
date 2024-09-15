#include "GLTextureBuffer.h"
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
    GUARD_LOG(renderEngine, "Missing render engine")

    auto glRenderEngine = static_cast<GLRenderEngine*>(renderEngine.get());

    glRenderEngine->BindFrameBuffer(frameBuffer.id);
    glViewport(0, 0, size.x, size.y);
}

void GLTextureBuffer::Clear() {
    // FUTURE: support stencil buffer if needed
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLTextureBuffer::Build(Vector2Int size) {
    GUARD_LOG(renderEngine, "Missing render engine")
    GUARD_LOG(!frameBuffer.id && !depthBuffer.id, "Can't rebuild existing buffer")

    auto glRenderEngine = static_cast<GLRenderEngine*>(renderEngine.get());

    glGenFramebuffers(1, &frameBuffer.id);
    glRenderEngine->BindFrameBuffer(frameBuffer.id);
    renderId = frameBuffer.id;

    GLuint textureId;
    glGenTextures(1, &textureId);

    texture = MAKE<GLTexture>("buffer", textureId, size, "");

    glRenderEngine->BindTexture2D(textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // FUTURE: evaluate advantages of using GL_DRAW_FRAMEBUFFER here
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glGenRenderbuffers(1, &depthBuffer.id);
    glRenderEngine->BindRenderBuffer(GL_RENDERBUFFER, depthBuffer.id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);

    // FUTURE: evaluate advantages of using GL_DRAW_FRAMEBUFFER here
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer.id);

    GUARD(CheckFrameBufferStatus())
    this->size = size;
}
