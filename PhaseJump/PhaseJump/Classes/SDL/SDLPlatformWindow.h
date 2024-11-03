#pragma once

#include "SDLPlatformWindowConfig.h"
#include "SDLWorld.h"
#include "SomePlatformWindow.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    /// Uses SDL to display a platform window
    class SDLPlatformWindow : public SomePlatformWindow {
    public:
        using Config = SDLPlatformWindowConfig;

    protected:
        SDL_Window* window{};
        SDL_Renderer* renderer{};

    public:
        using Base = SomePlatformWindow;

        SDLPlatformWindow(SP<SDLWorld> world) :
            Base(world) {}

        virtual ~SDLPlatformWindow() {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
        }

        void Configure(Config& config);

        SP<SDLWorld> World() const {
            return world;
        }

        void SetWorld(SP<SDLWorld> world) {
            this->world = world;
        }

        SDL_Window* SDLWindow() const {
            return window;
        }

        SDL_Renderer* SDLRenderer() const {
            return renderer;
        }

        float PixelDensity() const {
            GUARDR(window, 0)
            return SDL_GetWindowPixelDensity(window);
        }

        // MARK: SomePlatformWindow

        Vector2Int Size() const override {
            GUARDR(window, {})

            int x, y;
            SDL_GetWindowSize(window, &x, &y);
            return Vector2Int(x, y);
        }

        Vector2Int PixelSize() const override {
            GUARDR(window, {})

            int x, y;
            SDL_GetWindowSizeInPixels(window, &x, &y);
            return Vector2Int(x, y);
        }
    };
} // namespace PJ
