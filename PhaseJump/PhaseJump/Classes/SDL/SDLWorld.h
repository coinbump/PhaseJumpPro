#ifndef PJSDLWORLD_H
#define PJSDLWORLD_H

#include "World.h"
#include "SomeRenderer.h"
#include "SDLTextureRenderer.h"
#include "SDLEventPoller.h"
#include "VectorList.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <memory>

namespace PJ {
    class SDLWorld : public World {
    public:
        using Base = World;

    protected:
        bool isDone = false;
        SDL_Window *window;
        uint64_t startTime = 0;

    public:
        SDLWorld() {
        }

        SDL_Window* SDL_Window() const { return window; }

        virtual void Configure(struct SDL_Window *window, SP<SomeRenderContext> renderContext) {
            this->window = window;
            this->renderContext = renderContext;
        }

        void Run() {
            startTime = SDL_GetTicks64();

            while (!isDone) {
                auto currentTime = SDL_GetTicks64();
                double deltaTime = (currentTime - startTime) / 1000.0; // Convert to seconds.
                startTime = currentTime;

                OnUpdate(TimeSlice(deltaTime));

                Render();

                MainLoop();
            }
            SDL_Quit();
        }

    protected:
        void GoInternal() override {
            Base::GoInternal();
        }

        void MainLoop();
    };
}

#endif
