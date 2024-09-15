#ifndef PJSDLRENDERCONTEXT_H
#define PJSDLRENDERCONTEXT_H

#include "Color.h"
#include "FilePath.h"
#include "SDLLoadSDLTextureOperation.h"
#include "SDLTexture.h"
#include "SomeRenderContext.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

// CODE REVIEW: ?/23
namespace PJ {
    /// For rendering via `SDL_Renderer`
    //    class SDLRenderContext : public SomeRenderContext {
    //    public:
    //        SDL_Renderer* renderer = nullptr;
    //        Color clearColor = Color::gray;
    //
    //        SDLRenderContext(SDL_Renderer* renderer) :
    //            renderer(renderer) {}
    //
    //        virtual ~SDLRenderContext() {
    //            // Renderer is owned by window
    //        }
    //
    //        void Bind() override {}
    //
    //        void Clear() override {
    //            SDL_SetRenderDrawColor(
    //                renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a
    //            );
    //            SDL_RenderClear(renderer);
    //        }
    //
    //        void Present() override {
    //            SDL_RenderPresent(renderer);
    //        }
    //
    //        Vector2 Size() const override {
    //            int width, height;
    //            SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
    //            return Vector2(width, height);
    //        }
    //
    //        Vector2Int PixelSize() const override {
    //            int width, height;
    //            SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
    //            return Vector2Int(width, height);
    //        }
    //    };
} // namespace PJ

#endif
