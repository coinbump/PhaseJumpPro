#pragma once

#include "List.h"
#include "SomeUIEvent.h"
#include "VectorList.h"
#include <memory>
#include <SDL3/SDL.h>

// CODE REVIEW: ?/23
namespace PJ {
    class SomeUIEvent;
    class World;

    // TODO: vecotrList?
    using SDLEventList = List<SDL_Event>;

    struct SDLUIEventsBuilder {
        // TODO: Use UP here?
        UIEventList BuildUIEvents(SDLEventList const& events, float uiScale);
    };
} // namespace PJ
