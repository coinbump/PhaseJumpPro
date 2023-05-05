#ifndef PJSDLLOADSDLTEXTUREOPERATION_H
#define PJSDLLOADSDLTEXTUREOPERATION_H

#include "FilePath.h"
#include "SomeOperation.h"
#include "SDLTexture.h"
#include "Macros.h"
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    /// Use SDL to load an SDLTexture (use with `SDL_Renderer` only, not OpenGL context)
    class SDLLoadSDLTextureOperation : public SomeOperation {
    protected:
        SDL_Renderer *renderer;
        FilePath path;

    public:
        using Base = SomeOperation;

        SP<SDLTexture> texture;

        SDLLoadSDLTextureOperation(SDL_Renderer *renderer, FilePath path) : renderer(renderer), path(path) {
        }

        void GoInternal() override {
            Base::GoInternal();

            auto result = IMG_LoadTexture(renderer, path.string().c_str());
            if (result) {
                SDL_Point size;
                SDL_QueryTexture(result, nullptr, nullptr, &size.x, &size.y);
                texture = MAKE<SDLTexture>(result, Vector2Int(size.x, size.y));
            }
        }
    };
}

#endif
