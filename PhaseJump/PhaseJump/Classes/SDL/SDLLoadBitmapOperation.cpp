#include "SDLLoadBitmapOperation.h"
#include "Bitmap.h"
#include "SDLFileManager.h"
#include "SDLSurface.h"
#include "StringUtils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

using namespace std;
using namespace PJ;

SomeLoadResourcesOperation::Result SDLLoadBitmapOperation::LoadResources() {
    auto path = info.filePath;

    SDLSurface _surface{ { .surface = IMG_Load(path.string().c_str()) } };
    if (!_surface.Surface()) {
        PJ::Log(std::format("ERROR. Can't load image at: {}", path.string()));
        return Failure();
    }

    // Use ABGR here because on little endian systems, byte order is reversed
    SDLSurface convertedSurface{
        { .surface = SDL_ConvertSurface(_surface.Surface(), SDL_PIXELFORMAT_ABGR8888) }
    };
    if (!_surface.Surface()) {
        PJ::Log("ERROR. Can't convert image");
        return Failure();
    }

    auto surface = convertedSurface.Surface();

    auto pixelFormatDetails = SDL_GetPixelFormatDetails(surface->format);
    GUARDR(pixelFormatDetails, Failure())

    using SDLColor = BGRAColor;

    /// Wraps SDL pixel format details with some utility code
    struct SDLPixelFormat {
        using Pixel = SDLColor;

        SDL_PixelFormatDetails const* details{};

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

    auto pixelCount = surface->w * surface->h;
    auto rgbaPixelsDataSize = sizeof(uint32_t) * pixelCount;
    uint32_t* surfacePixel = (uint32_t*)surface->pixels;

    auto rgbaBitmap = MAKE<RGBABitmap>(RGBABitmap::Config{ .size = { surface->w, surface->h },
                                                           .pixels = surfacePixel,
                                                           .pixelsDataSize = rgbaPixelsDataSize });

    SDLFileManager fm;
    String id = fm.FileName(path, false);

    // Unless an id is specified, use the file name as an id
    auto info = this->info;
    if (IsEmpty(info.id)) {
        info.id = id;
    }

    return Success(ResourceModel{ .resource = rgbaBitmap, .info = info });
}
