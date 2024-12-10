#pragma once

#include "SomeTexture.h"
#include <fstream>
#include <SDL3/SDL_render.h>

/*
 RATING: 5 stars
 Simple wrapper
 CODE REVIEW: 12/1/24
 */
namespace PJ {
    class SDLTexture : public SomeTexture {
    public:
        using Base = SomeTexture;

        SDL_Texture* texture{};

        SDLTexture(SDL_Texture* texture, Vector2Int size) :
            Base({ .size = size }),
            texture(texture) {}

        virtual ~SDLTexture() {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
        }
    };
} // namespace PJ
