#ifndef PJSDLUIEVENTSBUILDER_H
#define PJSDLUIEVENTSBUILDER_H

#include "VectorList.h"
#include "List.h"
#include <memory>
#include <SDL2/SDL.h>

namespace PJ {
    class SomeUIEvent;

    using SDLEventList = List<SDL_Event>;

    struct SDLUIEventsBuilder {
        VectorList<SP<SomeUIEvent>> BuildUIEvents(SDLEventList const& events);
    };
}

#endif
