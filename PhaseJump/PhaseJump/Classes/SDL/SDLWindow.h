#ifndef PJSDLWINDOW_H
#define PJSDLWINDOW_H

#include "SomeWindow.h"
#include "SDLWorld.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace PJ
{
    class SDLWindow : public SomeWindow {
    protected:
        SDL_Renderer* renderer;
        SDL_Window* window;
        std::shared_ptr<SDLWorld> world;

    public:
        using Base = SomeWindow;

        SDLWindow(Vector2Int size) : Base(size)
        {
        }

        virtual ~SDLWindow() {
            SDL_DestroyWindow(window);
            window = nullptr;
            renderer = nullptr;
        }

        SDL_Renderer* Renderer() const { return renderer; }

        std::shared_ptr<SDLWorld> World() const { return world; }

    protected:
        void GoInternal() override {
            Base::GoInternal();

            if (SDL_CreateWindowAndRenderer(size.x(), size.y(), 0, &window, &renderer) < 0) {
                std::cout << "ERROR: Can't create SDL Window.";
                return;
            }

            world = std::make_shared<SDLWorld>(std::make_shared<SDLRenderContext>(renderer));

            // Important: Call world->Go after you've built your scene
        }
    };
}

#endif
