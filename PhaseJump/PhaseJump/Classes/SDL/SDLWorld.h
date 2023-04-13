#ifndef PJSDLWORLD_H
#define PJSDLWORLD_H

#include "World.h"
#include "SomeRenderer.h"
#include "SDLTextureRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <memory>

namespace PJ {
    class SDLEventPoller : public SomeUIEventPoller {
    public:
        Status PollUIEvents() override {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT
                    || (event.type == SDL_WINDOWEVENT
                        && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
                    // && event.window.windowID == SDL_GetWindowID(*window))) {
                    return Status::Done;
                }
            }

            return Status::Running;
        }
    };

    class SDLWorld : public World {
    public:
        using Base = World;

    protected:
        bool isDone = false;
        
    public:
        SDLWorld(std::shared_ptr<SomeRenderContext> renderContext) {
            this->renderContext = renderContext;
            this->uiEventPoller = std::make_shared<SDLEventPoller>();
        }

    protected:
        void GoInternal() override {
            Base::GoInternal();

            while (!isDone) {
                mainLoop();
            }
            SDL_Quit();
        }

        void mainLoop()
        {
            if (!renderContext) { return; }

            // TODO: temp code
            // TODO: dispatch to event system
            auto status = uiEventPoller->PollUIEvents();
            isDone = status == SomeUIEventPoller::Status::Done;

            Render();
        }
    };
}

#endif
