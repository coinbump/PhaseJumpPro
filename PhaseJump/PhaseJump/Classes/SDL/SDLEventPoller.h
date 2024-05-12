#ifndef PJSDLEVENTPOLLER_H
#define PJSDLEVENTPOLLER_H

#include "SomeUIEventPoller.h"
#include "SDLUIEventsBuilder.h"
#include <SDL2/SDL.h>
#include <memory>

namespace PJ {
    class SDLEventPoller : public SomeUIEventPoller {
    public:
        SDL_Window* window = nullptr;

        SDLEventPoller(SDL_Window* window) : window(window) {
        }

        Result PollUIEvents() override;
        Result Process(SDLEventList const& events);
    };
}

#endif
