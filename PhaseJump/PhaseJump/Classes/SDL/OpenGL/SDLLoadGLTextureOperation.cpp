#include "SDLLoadGLTextureOperation.h"
#include "Bitmap.h"
#include "GLTexture.h"
#include "StringUtils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

using namespace std;
using namespace PJ;

SomeLoadResourcesOperation::Result SDLLoadGLTextureOperation::LoadResources() {
    auto path = info.filePath;

    SDL_Surface* surface = IMG_Load(path.string().c_str());
    if (!surface) {
        PJLog("ERROR. Can't load GL texture at: %s", path.string().c_str());
        return Failure();
    }

    // TODO: evaluate using SDL's GL_CreateTexture instead

    int surfacePixelSize = 4;
    GLenum pixelFormat = GL_BGR;
    switch (surface->format) {
    case SDL_PixelFormat::SDL_PIXELFORMAT_ARGB8888:
        pixelFormat = GL_BGRA;
        surfacePixelSize = 4;
        break;
    default:
        PJLog("Only RGBA supported for now.");
        return Failure();
        break;
    }

    auto surfaceDataSize = surfacePixelSize * surface->w * surface->h;
    BGRABitmap bgraBitmap(Vector2Int(surface->w, surface->h), surface->pixels, surfaceDataSize);
    RGBABitmap rgbaBitmap(Vector2Int(surface->w, surface->h));

    rgbaBitmap.Pixels().clear();
    std::copy(
        bgraBitmap.Pixels().begin(), bgraBitmap.Pixels().end(),
        std::back_inserter(rgbaBitmap.Pixels())
    );

    // SDL does not load textures premultiplied, so if we want that, we need to
    // apply it Premultiply
    for (auto& pixel : rgbaBitmap.Pixels()) {
        Color color(pixel);
        color.r *= color.a;
        color.g *= color.a;
        color.b *= color.a;
        pixel = (RGBAColor)color;
    }

    // http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
    GLuint glTexture = 0;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    GLint glTextureMag = GLTexture::GLTextureMagnification(textureMagnification);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glTextureMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int width = surface->w;
    int height = surface->h;

    // https://stackoverflow.com/questions/13666196/why-do-images-loaded-to-texture-with-img-load-in-sdl-and-opengl-look-bluish
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, rgbaBitmap.Size().x, rgbaBitmap.Size().y, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, rgbaBitmap.Pixels().data()
    );

    String id = path.filename().string();
    auto components = ComponentsSeparatedBy(id, '.');
    if (components.size() > 0) {
        id = components[0];
    }

    auto texture =
        MAKE<GLTexture>(id, glTexture, Vector2Int(width, height), TextureAlphaMode::Standard);
    texture->SomeTexture::SetTextureMagnification(textureMagnification);

    LoadedResource loadedResource(info.filePath, info.type, info.id, SCAST<PJ::Base>(texture));
    Success result;
    result.Add(loadedResource);

    SDL_DestroySurface(surface);

    return result;
}
