#ifndef PJSDL_TEXTURE_H
#define PJSDL_TEXTURE_H

#include "SomeTexture.h"
#include <SDL2/SDL_render.h>

#include <fstream>

/*
 RATING: 5 stars
 Simple wrapper
 CODE REVIEW: 12/24/22
 */
namespace PJ
{
    class SDLTexture : public SomeTexture {
    public:
        SDL_Texture* texture = nullptr;

        using Base = SomeTexture;

        SDLTexture(SDL_Texture* texture, Vector2Int size) : Base("", 0, size, TextureAlphaMode::Standard), texture(texture) {
        }

        virtual ~SDLTexture() {
            SDL_DestroyTexture(texture);
        }
    };
}

#endif
