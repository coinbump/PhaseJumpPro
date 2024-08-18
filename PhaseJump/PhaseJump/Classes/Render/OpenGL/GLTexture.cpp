#include "GLTexture.h"

using namespace std;
using namespace PJ;

void GLTexture::SetTextureMagnification(TextureMagnificationType value) {
    GLint glTextureMag = GLTextureMagnification(value);

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
}

void GLTexture::SetTextureWrap(TextureWrapType value) {
    GLint glTextureWrap = GLTextureWrap(value);

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTextureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTextureWrap);
}
