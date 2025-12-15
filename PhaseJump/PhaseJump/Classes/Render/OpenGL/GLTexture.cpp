#include "GLTexture.h"

using namespace std;
using namespace PJ;

GLTexture::GLTexture(Config const& config) :
    Base(config) {
    if (config.bitmap) {
        auto& bitmap = *config.bitmap;

        // http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
        renderId = 0;
        glGenTextures(1, &renderId);
        glBindTexture(GL_TEXTURE_2D, renderId);

        GLint glTextureMag = GLTextureMagnificationFor(config.textureMagnification);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /*
         FUTURE: determine best pixel format for each platform
         glGetInternalFormativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1,
         &preferred_format)

         Reference:

         https://www.khronos.org/opengl/wiki/Common_Mistakes

         Reference:

         https://stackoverflow.com/questions/13666196/why-do-images-loaded-to-texture-with-img-load-in-sdl-and-opengl-look-bluish
         */
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, bitmap.Size().x, bitmap.Size().y, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, bitmap.Pixels().data()
        );

        size = bitmap.Size();
        untrimmedSize = size;
    } else {
        SetTextureMagnification(config.textureMagnification);
    }
}

void GLTexture::SetTextureMagnification(TextureMagnificationType value) {
    GUARD(renderId)

    GLint glTextureMag = GLTextureMagnificationFor(value);

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
}

void GLTexture::SetTextureWrap(TextureWrapType value) {
    GUARD(renderId)

    GLint glTextureWrap = GLTextureWrapFor(value);

    glBindTexture(GL_TEXTURE_2D, renderId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTextureWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTextureWrap);
}

GLTexture::~GLTexture() {
    if (renderId > 0) {
        glDeleteTextures(1, &renderId);
    }
}

GLint PJ::GLTextureMagnificationFor(TextureMagnificationType textureMagnification) {
    OrderedMap<TextureMagnificationType, GLint> map = {
        { TextureMagnify::Nearest, GL_NEAREST },
        { TextureMagnify::Linear, GL_LINEAR },
    };
    auto i = map.find(textureMagnification);
    GUARDR(i != map.end(), GL_LINEAR);
    return i->second;
}

GLint PJ::GLTextureWrapFor(TextureWrapType textureWrap) {
    OrderedMap<TextureWrapType, GLint> map = {
        { TextureWrap::Clamp, GL_CLAMP_TO_EDGE },
        { TextureWrap::Repeat, GL_REPEAT },
    };
    auto i = map.find(textureWrap);
    GUARDR(i != map.end(), GL_CLAMP_TO_EDGE);
    return i->second;
}
