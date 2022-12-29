#ifndef PJSDLLOADTEXTUREOPERATION_H
#define PJSDLLOADTEXTUREOPERATION_H

#include "FilePath.h"
#include "SomeOperation.h"
#include "SDLTexture.h"
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/24/22
 */
namespace PJ {
    class SDLLoadTextureOperation : public SomeOperation {
    protected:
        SDL_Renderer *renderer;
        FilePath path;

    public:
        using Base = SomeOperation;

        std::shared_ptr<SDLTexture> texture;

        SDLLoadTextureOperation(SDL_Renderer *renderer, FilePath path) : renderer(renderer), path(path) {
        }

        void GoInternal() override {
            Base::GoInternal();

            FilePath path = SDL_GetBasePath();
            path += FilePath(this->path);

            auto result = IMG_LoadTexture(renderer, path.string().c_str());
            if (result) {
                SDL_Point size;
                SDL_QueryTexture(result, nullptr, nullptr, &size.x, &size.y);
                texture = make_shared<SDLTexture>(result, Vector2Int(size.x, size.y));
            }
        }
    };
}

#endif
