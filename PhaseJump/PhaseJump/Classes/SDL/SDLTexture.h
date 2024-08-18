#ifndef PJSDL_TEXTURE_H
#define PJSDL_TEXTURE_H

#include "SomeTexture.h"
#include <fstream>
#include <SDL3/SDL_render.h>

/*
 RATING: 5 stars
 Simple wrapper
 CODE REVIEW: 7/5/24
 */
namespace PJ {
    class SDLTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        SDL_Texture* texture = nullptr;

        SDLTexture(SDL_Texture* texture, Vector2Int size) :
            Base("", 0, size, TextureAlphaMode::Standard),
            texture(texture) {}

        virtual ~SDLTexture() {
            SDL_DestroyTexture(texture);
        }
    };
} // namespace PJ

#endif
