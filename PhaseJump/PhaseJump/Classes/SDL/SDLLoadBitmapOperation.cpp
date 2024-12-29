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
        PJ::Log("ERROR. Can't load GL texture at: ", path.string());
        return Failure();
    }

    auto surface = _surface.Surface();

    // FUTURE: evaluate using SDL_CreateTexture instead

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

    // FUTURE: support non 32-bit color formats
    if (pixelFormat.PixelSize() != sizeof(uint32_t)) {
        PJ::Log("ERROR. Only 32-bit images supported");
        return Failure();
    }

    auto surfaceColorSchema = pixelFormat.MakeColorSchema();
    auto rgbaPixelsDataSize = sizeof(uint32_t) * pixelCount;
    uint32_t* rgbaPixels = (uint32_t*)malloc(rgbaPixelsDataSize);
    uint32_t* surfacePixel = (uint32_t*)surface->pixels;
    uint32_t* rgbaPixel = rgbaPixels;

    for (int i = 0; i < pixelCount; i++) {
        *rgbaPixel = ComponentColor32(*surfacePixel).ConvertTo<RGBAColor>(surfaceColorSchema).value;
        surfacePixel++;
        rgbaPixel++;
    }

    auto rgbaBitmap = MAKE<RGBABitmap>(RGBABitmap::Config{ .size = { surface->w, surface->h },
                                                           .pixels = rgbaPixels,
                                                           .pixelsDataSize = rgbaPixelsDataSize });

    // Pixels are copied into Bitmap, free the original
    free(rgbaPixels);

    SDLFileManager fm;
    String id = fm.FileName(path, false);

    // Unless an id is specified, use the file name as an id
    auto info = this->info;
    if (IsEmpty(info.id)) {
        info.id = id;
    }

    return Success(ResourceModel{ .resource = rgbaBitmap, .info = info });
}
