#include "SDLLoadGLTextureOperation.h"

using namespace std;
using namespace PJ;

SDLLoadGLTextureOperation::SDLLoadGLTextureOperation(FilePath path, TextureMagnification textureMagnification) : path(path), textureMagnification(textureMagnification) {
}

void SDLLoadGLTextureOperation::GoInternal() {
    Base::GoInternal();

    SDL_Surface *surface = IMG_Load(path.string().c_str());
    if (!surface) {
        PJLog("ERROR. Can't load GL texture at: %s", path.string().c_str());
        return;
    }

    GLenum pixelFormat = GL_BGR;
    if (surface->format->BytesPerPixel == 4) {
        pixelFormat = GL_BGRA;
    }

    // http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
    GLuint glTexture = 0;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    GLint glTextureMag = GL_LINEAR;
    switch (textureMagnification) {
        case TextureMagnification::Nearest:
            glTextureMag = GL_NEAREST;
            break;
        case TextureMagnification::Linear:
            glTextureMag = GL_LINEAR;
            break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int width = surface->w;
    int height = surface->h;

    // https://stackoverflow.com/questions/13666196/why-do-images-loaded-to-texture-with-img-load-in-sdl-and-opengl-look-bluish
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

    this->texture = std::make_shared<GLTexture>(glTexture, Vector2Int(width, height));

    SDL_FreeSurface(surface);
}
