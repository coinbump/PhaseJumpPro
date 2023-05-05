#include "GLTexture.h"

using namespace std;
using namespace PJ;

void GLTexture::SetTextureMagnification(TextureMagnification value) {
    GLint glTextureMag = GL_LINEAR;
    switch (value) {
        case TextureMagnification::Nearest:
            glTextureMag = GL_NEAREST;
            break;
        case TextureMagnification::Linear:
            glTextureMag = GL_LINEAR;
            break;
    }

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
}

void GLTexture::SetTextureWrap(TextureWrap value) {
    GLint glTextureWrap = GL_CLAMP_TO_EDGE;
    switch (value) {
        case TextureWrap::Clamp:
            glTextureWrap = GL_CLAMP_TO_EDGE;
            break;
        case TextureWrap::Repeat:
            glTextureWrap = GL_REPEAT;
            break;
    }

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTextureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTextureWrap);
}
