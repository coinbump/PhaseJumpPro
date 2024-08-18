#ifndef PJSDLEVENTPOLLER_H
#define PJSDLEVENTPOLLER_H

#include "SDLUIEventsBuilder.h"
#include "SomeUIEventPoller.h"
#include <memory>
#include <SDL3/SDL.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SDLEventPoller : public SomeUIEventPoller {
    public:
        SDL_Window* window = nullptr;

        SDLEventPoller(SDL_Window* window) :
            window(window) {}

        Result PollUIEvents() override;
        Result Process(SDLEventList const& events);
    };
} // namespace PJ

#endif
