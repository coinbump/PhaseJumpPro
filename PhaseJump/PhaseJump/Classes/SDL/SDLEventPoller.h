#ifndef PJSDLEVENTPOLLER_H
#define PJSDLEVENTPOLLER_H

#include "SomeUIEventPoller.h"
#include "SDLUIEventBuilder.h"
#include <SDL2/SDL.h>
#include <memory>

namespace PJ {
    class SDLEventPoller : public SomeUIEventPoller {
    public:
        SDL_Window* window = nullptr;

        SDLEventPoller(SDL_Window* window) : window(window) {
        }

        Result PollUIEvents() override {
            VectorList<std::shared_ptr<SomeUIEvent>> uiEvents;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                VectorList<std::shared_ptr<SomeUIEvent>> thisUIEvents = SDLUIEventBuilder().BuildUIEvents(event);
                uiEvents.AddRange(thisUIEvents);

                if (event.type == SDL_QUIT
                    || (event.type == SDL_WINDOWEVENT
                        && event.window.event == SDL_WINDOWEVENT_CLOSE
                        && event.window.windowID == SDL_GetWindowID(window))) {
                    return Result(Status::Done, uiEvents);
                }
            }

            return Result(Status::Running, uiEvents);
        }
    };
}

#endif
