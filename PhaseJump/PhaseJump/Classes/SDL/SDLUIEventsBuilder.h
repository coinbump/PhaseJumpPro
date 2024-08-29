#pragma once

#include "List.h"
#include "VectorList.h"
#include <memory>
#include <SDL3/SDL.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeUIEvent;

    using SDLEventList = List<SDL_Event>;

    struct SDLUIEventsBuilder {
        List<SP<SomeUIEvent>> BuildUIEvents(SDLEventList const& events);
    };
} // namespace PJ
