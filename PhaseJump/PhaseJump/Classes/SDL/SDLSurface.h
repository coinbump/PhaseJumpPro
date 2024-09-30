#pragma once

#include "Bitmap.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

namespace PJ {
    struct SDLSurface {
        SDL_Surface* surface{};

        SDLSurface(SDL_Surface* surface) :
            surface(surface) {}

        ~SDLSurface() {
            SDL_DestroySurface(surface);
        }
    };

    // Important: this is only guaranteed to work with PixelFormat::RGBA8888
    // FUTURE: support more pixel formats as needed
    template <class PixelFormat = PixelFormat::RGBA8888>
    SP<SDLSurface> MakeSurface(Bitmap<PixelFormat> const& bitmap) {
        auto bitmapData = bitmap.Data();

        /// SDL requires us to create the surface buffer and copy in pixels
        void* sdlPixelData = malloc(bitmapData.size_bytes());
        memcpy(sdlPixelData, bitmapData.data(), bitmapData.size_bytes());

        auto sdlSurface = SDL_CreateSurfaceFrom(
            bitmap.Size().x, bitmap.Size().y, SDL_PIXELFORMAT_RGBA8888, sdlPixelData,
            bitmap.Size().x * PixelFormat().PixelSize()
        );
        GUARDR(sdlSurface, nullptr)

        return MAKE<SDLSurface>(sdlSurface);
    }

} // namespace PJ
