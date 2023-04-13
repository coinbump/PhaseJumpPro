#ifndef PJSDLRENDERCONTEXT_H
#define PJSDLRENDERCONTEXT_H

#include "SomeRenderContext.h"
#include "Color.h"
#include "FilePath.h"
#include "SDLTexture.h"
#include "SDLLoadTextureOperation.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <iostream>

namespace PJ
{
    /// For rendering via `SDL_Renderer`
    class SDLRenderContext : public SomeRenderContext
    {
    public:
        SDL_Renderer* renderer = nullptr;
        Color clearColor = Color::gray;

        SDLRenderContext(SDL_Renderer *renderer) : renderer(renderer)
        {
        }

        virtual ~SDLRenderContext() {
            // Renderer is owned by window
        }

        void Bind() override {
        }

        void Clear() override {
            Color32 clearColor = this->clearColor;
            SDL_SetRenderDrawColor(renderer, clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
            SDL_RenderClear(renderer);
        }

        void Present() override {
            SDL_RenderPresent(renderer);
        }

        Vector2 Size() const override
        {
            int width, height;
            SDL_GetRendererOutputSize(renderer, &width, &height);
            return Vector2(width, height);
        }

        Vector2Int PixelSize() const override
        {
            int width, height;
            SDL_GetRendererOutputSize(renderer, &width, &height);
            return Vector2Int(width, height);
        }
    };
}

#endif
