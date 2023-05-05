#include "SDLLoadGLTextureOperation.h"
#include "GLTexture.h"
#include "Bitmap.h"

using namespace std;
using namespace PJ;

SDLLoadGLTextureOperation::SDLLoadGLTextureOperation(FilePath filePath, TextureMagnification textureMagnification) : textureMagnification(textureMagnification) {
    this->info.filePath = filePath;
}

void SDLLoadGLTextureOperation::Run() {
    auto path = info.filePath;

    SDL_Surface *surface = IMG_Load(path.string().c_str());
    if (!surface) {
        PJLog("ERROR. Can't load GL texture at: %s", path.string().c_str());
        return;
    }

    GLenum pixelFormat = GL_BGR;
    if (surface->format->BytesPerPixel == 4) {
        pixelFormat = GL_BGRA;
    } else {
        PJLog("Only RGBA supported for now.");
        return;
    }

    BGRABitmap bgraBitmap(Vector2Int(surface->w, surface->h), surface->pixels);
    RGBABitmap rgbaBitmap(Vector2Int(surface->w, surface->h));

    rgbaBitmap.Pixels().clear();
    std::copy(bgraBitmap.Pixels().begin(), bgraBitmap.Pixels().end(), std::back_inserter(rgbaBitmap.Pixels()));

    // SDL does not load textures premultiplied, so if we want that, we need to apply it
    // Premultiply
    for (auto & pixel : rgbaBitmap.Pixels()) {
        Color color(pixel);
        color.r *= color.a;
        color.g *= color.a;
        color.b *= color.a;
        pixel = (Color32)color;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaBitmap.Size().x, rgbaBitmap.Size().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBitmap.Pixels().data());

    String id = path.filename().string();
    auto components = id.ComponentsSeparatedBy('.');
    if (components.size() > 0) {
        id = components[0];
    }

    auto texture = MAKE<GLTexture>(id, glTexture, Vector2Int(width, height), TextureAlphaMode::Standard);

    LoadedResource loadedResource(info.filePath, info.typeId, info.id, static_pointer_cast<PJ::Base>(texture));
    Success result;
    result.Add(loadedResource);

    this->result = MAKE<Result>(result);

    SDL_FreeSurface(surface);
}
