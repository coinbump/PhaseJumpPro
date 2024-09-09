#include "GLTextureBuffer.h"
#include "GLTexture.h"

using namespace std;
using namespace PJ;

GLTextureBuffer::~GLTextureBuffer() {
    if (frameBufferId) {
        glDeleteFramebuffers(1, &frameBufferId);
    }

    if (depthBufferId) {
        glDeleteRenderbuffers(1, &depthBufferId);
    }

    // Texture object deletes the texture
}

void CheckFrameBufferStatus() {
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        PJLog("GL Texture buffer format unsupported %x", status);
        break;
    default:
        PJLog("Failed to build GL texture buffer %x", status);
        break;
    }
}

void GLTextureBuffer::Bind() {
    GUARD(frameBufferId)

    // TODO: dispatch all calls through the render engine
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0, 0, size.x, size.y);
}

void GLTextureBuffer::Clear() {
    // FUTURE: support stencil buffer
    glClearColor(1, 0, 1, 1);
    // TODO: glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLTextureBuffer::Build(Vector2Int size) {
    if (frameBufferId > 0) {
        PJLog("ERROR. Can't rebuild an existing texture buffer");
        return;
    }

    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    glGenTextures(1, &textureId);

    texture = MAKE<GLTexture>("buffer", textureId, size, "");

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

    glGenRenderbuffers(1, &depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x, size.y);
    glFramebufferRenderbuffer(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId
    );

    CheckFrameBufferStatus();
}
