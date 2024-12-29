#pragma once

#include "List.h"
#include "SomeUIEvent.h"
#include "VectorList.h"
#include <memory>
#include <SDL3/SDL.h>

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    class SomeUIEvent;
    class World;

    using SDLEventList = VectorList<SDL_Event>;

    /// Converts a list of SDL events into their corresponding PhaseJump event objects
    struct SDLUIEventsBuilder {
        UIEventList BuildUIEvents(SDLEventList const& events, float uiScale);
    };
} // namespace PJ
