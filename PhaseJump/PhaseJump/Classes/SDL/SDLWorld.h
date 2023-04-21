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

    public:
        SDLWorld() {
        }

        SDL_Window* SDL_Window() const { return window; }

        virtual void Configure(struct SDL_Window *window, std::shared_ptr<SomeRenderContext> renderContext) {
            this->window = window;
            this->renderContext = renderContext;
        }

    protected:
        void GoInternal() override {
            Base::GoInternal();

            while (!isDone) {
                mainLoop();
            }
            SDL_Quit();
        }

        void mainLoop();
        virtual void ProcessUIEvents(VectorList<std::shared_ptr<SomeUIEvent>> const& uiEvents);
    };
}

#endif
