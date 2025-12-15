#pragma once

#include "Bitmap.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/10/24
 */
namespace PJ {
    /**
     Stores SDL surface and pixel data

     `SDL_Surface` doesn't manage the pixel data, we are responsible for freeing it after the
     surface is freed Reference: https://wiki.libsdl.org/SDL3/SDL_CreateSurfaceFrom
     */
    class SDLSurface {
    protected:
        SDL_Surface* surface{};
        void* pixelData{};

    public:
        struct Config {
            SDL_Surface* surface{};
            void* pixelData{};

            /// Method: create surface with bitmap data
            RGBABitmap* bitmap{};
        };

        SDLSurface(Config const& config);

        SDL_Surface* Surface() const {
            return surface;
        }

        ~SDLSurface();
    };
} // namespace PJ
