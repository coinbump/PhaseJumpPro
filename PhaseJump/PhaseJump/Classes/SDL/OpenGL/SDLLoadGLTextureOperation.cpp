#include "SDLLoadGLTextureOperation.h"
#include "Bitmap.h"
#include "GLTexture.h"
#include "SDLSurface.h"
#include "StringUtils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

using namespace std;
using namespace PJ;

SomeLoadResourcesOperation::Result SDLLoadGLTextureOperation::LoadResources() {
    auto path = info.filePath;

    SDL_Surface* surface = IMG_Load(path.string().c_str());
    if (!surface) {
        PJ::Log("ERROR. Can't load GL texture at: ", path.string());
        return Failure();
    }

    // TODO: evaluate using SDL's GL_CreateTexture instead

    auto pixelFormatDetails = SDL_GetPixelFormatDetails(surface->format);
    GUARDR(pixelFormatDetails, Failure())

    // TODO: this is Unfinished. The problem is that ComponentColor requires a constant color schema
    auto details = pixelFormatDetails;
    ComponentColorSchema colorSchema{ details->Rmask,  details->Gmask,  details->Bmask,
                                      details->Amask,  details->Rshift, details->Gshift,
                                      details->Bshift, details->Ashift };
    using SDLColor = ComponentColor32<ComponentColorSchema::bgra>;

    struct SDLPixelFormat {
        using Pixel = SDLColor;

        SDL_PixelFormatDetails const* details = nullptr;

        size_t ColorComponentBitSize(ColorComponent component) const {
            switch (component) {
            case ColorComponent::Red:
                return details->Rbits;
            case ColorComponent::Green:
                return details->Gbits;
            case ColorComponent::Blue:
                return details->Bbits;
            case ColorComponent::Alpha:
                return details->Abits;
            }
        }

        size_t PixelSize() const {
            return details->bytes_per_pixel;
        }

        ComponentColorSchema MakeColorSchema() {
            return { details->Rmask,  details->Gmask,  details->Bmask,  details->Amask,
                     details->Rshift, details->Gshift, details->Bshift, details->Ashift };
        }
    };

    SDLPixelFormat pixelFormat(pixelFormatDetails);
    using SDLBitmap = Bitmap<SDLPixelFormat>;

    // TODO: use this information for loading images
    //    typedef struct SDL_PixelFormatDetails
    //    {
    //        Uint8 padding[2];
    //    } SDL_PixelFormatDetails;

    auto surfaceDataSize = pixelFormat.PixelSize() * surface->w * surface->h;
    BGRABitmap bgraBitmap(Vector2Int(surface->w, surface->h), surface->pixels, surfaceDataSize);
    RGBABitmap rgbaBitmap(Vector2Int(surface->w, surface->h));

    std::copy(bgraBitmap.Pixels().begin(), bgraBitmap.Pixels().end(), rgbaBitmap.Pixels().begin());

// #define TEST_MAKE_SURFACE
#ifdef TEST_MAKE_SURFACE
    // TODO: temp code
    auto testSurface = MakeSurface(rgbaBitmap);
    surfaceDataSize = pixelFormat.PixelSize() * testSurface->surface->w * testSurface->surface->h;
    RGBABitmap bgraBitmap2(
        Vector2Int(testSurface->surface->w, testSurface->surface->h), testSurface->surface->pixels,
        surfaceDataSize
    );

    std::copy(
        bgraBitmap2.Pixels().begin(), bgraBitmap2.Pixels().end(), rgbaBitmap.Pixels().begin()
    );
#endif

    // SDL does not load textures premultiplied, so if we want that, we need to
    // apply it Premultiply
    for (auto& pixel : rgbaBitmap.Pixels()) {
        Color color(pixel);
        color.r *= color.a;
        color.g *= color.a;
        color.b *= color.a;
        pixel = (RGBAColor)color;
    }

    // OpenGl expects textures to be upside-down
    // rgbaBitmap.FlipV();

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

    auto texture = MAKE<GLTexture>(
        id, glTexture, Vector2Int(width, height), vec2Zero, vec2One, TextureAlphaMode::Standard
    );
    texture->SomeTexture::SetTextureMagnification(textureMagnification);

    ResourceModel loadedResource(SCAST<PJ::Base>(texture), info.id, info.filePath, info.type);
    Success result;
    result.Add(loadedResource);

    SDL_DestroySurface(surface);

    return result;
}
