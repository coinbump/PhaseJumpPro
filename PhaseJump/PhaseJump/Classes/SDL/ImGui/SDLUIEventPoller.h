#pragma once

#include "SDLUIEventsBuilder.h"
#include "SomePosition.h"
#include "SomeUIEventPoller.h"
#include "VectorList.h"
#include <SDL3/SDL.h>

class SDL_Window;

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 10/20/24
 */
namespace PJ {
    /// Polls SDL UI events, with support for imGui
    class SDLUIEventPoller : public SomeUIEventPoller {
    protected:
        Result Process(SDLEventList const& events);

    public:
        SDL_Window* window{};

        SDLUIEventPoller(SDL_Window* window);

        virtual ~SDLUIEventPoller() {}

        // MARK: SomeUIEventPoller

        Result PollUIEvents() override;
    };
}; // namespace PJ
