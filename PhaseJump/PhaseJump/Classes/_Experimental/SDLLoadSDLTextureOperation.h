#pragma once

#include "FilePath.h"
#include "SDLTexture.h"
#include "SomeOperation.h"
#include "Utils.h"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

// Experimental
namespace PJ {
    /// Use SDL to load an SDLTexture (use with `SDL_Renderer` only, not OpenGL
    /// context)
    class SDLLoadSDLTextureOperation : public SomeOperation {
    protected:
        SDL_Renderer* renderer;
        FilePath path;

    public:
        using Base = SomeOperation;

        // Don't think this works, don't we have to call SDL_Destroy?
        SP<SDLTexture> texture;

        SDLLoadSDLTextureOperation(SDL_Renderer* renderer, FilePath path) :
            renderer(renderer),
            path(path) {}

        // MARK: Base

        void OnGo() override {
            Base::OnGo();

            SDL_Texture* result = IMG_LoadTexture(renderer, path.string().c_str());
            if (result) {
                //                auto props = SDL_GetTextureProperties(texture);
                //                auto width = SDL_GetNumberProperty(props,
                //                SDL_PROP_TEXTURE_WIDTH_NUMBER, 0); auto height =
                //                SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);

                float width, height;
                SDL_GetTextureSize(result, &width, &height);
                texture = MAKE<SDLTexture>(result, Vector2Int(width, height));
            }
        }
    };
} // namespace PJ
