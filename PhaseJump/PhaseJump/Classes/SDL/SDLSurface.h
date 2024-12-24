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
    struct SDLSurface {
        SDL_Surface* surface{};
        void* pixelData{};

        struct Config {
            SDL_Surface* surface{};
            void* pixelData{};

            /// If provided, the surface will be created with this bitmap
            SomeBitmap* bitmap{};
        };

        SDLSurface(Config config);

        ~SDLSurface();
    };
} // namespace PJ
