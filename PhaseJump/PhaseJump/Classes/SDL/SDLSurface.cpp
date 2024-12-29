#include "SDLSurface.h"

using namespace std;
using namespace PJ;

SDLSurface::SDLSurface(Config config) :
    surface(config.surface),
    pixelData(config.pixelData) {
    if (config.bitmap) {
        auto& bitmap = *config.bitmap;
        void* bitmapData = bitmap.DataPtr();
        auto sizeInBytes = bitmap.DataSize();

        /// SDL requires us to create the surface buffer and copy in pixels
        pixelData = malloc(sizeInBytes);
        memcpy(pixelData, bitmapData, sizeInBytes);

        // SDL uses literal byte order, so ABGR8888 is little endian RGBA
        surface = SDL_CreateSurfaceFrom(
            bitmap.Size().x, bitmap.Size().y, SDL_PIXELFORMAT_ABGR8888, pixelData,
            bitmap.Size().x * bitmap.PixelSize()
        );
    }
}

SDLSurface::~SDLSurface() {
    SDL_DestroySurface(surface);

    if (pixelData) {
        free(pixelData);
    }
}
