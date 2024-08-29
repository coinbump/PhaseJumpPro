#ifndef PJSDLWORLD_H
#define PJSDLWORLD_H

#include "DevProfiler.h"
#include "SDLEventPoller.h"
#include "SDLTextureRenderer.h"
#include "SomeRenderer.h"
#include "VectorList.h"
#include "World.h"
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

// #define PROFILE

// CODE REVIEW: ?/23
namespace PJ {
    // TODO: this should really be a system shouldn't it?
    class SDLWorld : public World {
    public:
        using Base = World;

    protected:
        bool isDone = false;
        SDL_Window* window;
        uint64_t startTime = 0;

    public:
        SDLWorld() {}

        SDL_Window* SDL_Window() const {
            return window;
        }

        virtual void Configure(struct SDL_Window* window, SP<SomeRenderContext> renderContext) {
            this->window = window;
            this->renderContext = renderContext;
        }

        void Run() {
            startTime = SDL_GetTicks();

            while (!isDone) {
                auto currentTime = SDL_GetTicks();
                double deltaTime = (currentTime - startTime) / 1000.0; // Convert to seconds.
                startTime = currentTime;

                {
#ifdef PROFILE
                    DevProfiler devProfiler("SDL-OnUpdate", [](String value) {
                        std::cout << value;
                    });
#endif

                    OnUpdate(TimeSlice(deltaTime));
                }

                {
#ifdef PROFILE
                    DevProfiler devProfiler("SDL-Render", [](String value) { std::cout << value; });
#endif
                    Render();
                }

                {
#ifdef PROFILE
                    DevProfiler devProfiler("SDL-MainLoop", [](String value) {
                        std::cout << value;
                    });
#endif
                    MainLoop();
                }
            }
            SDL_Quit();
        }

    protected:
        void GoInternal() override {
            Base::GoInternal();
        }

        void MainLoop();
    };
} // namespace PJ

#endif
