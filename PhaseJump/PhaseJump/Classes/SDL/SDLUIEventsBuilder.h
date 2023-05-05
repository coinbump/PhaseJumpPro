#ifndef PJSDLUIEVENTSBUILDER_H
#define PJSDLUIEVENTSBUILDER_H

#include "VectorList.h"
#include <memory>
#include <SDL2/SDL.h>

namespace PJ {
    class SomeUIEvent;

    struct SDLUIEventsBuilder {
        VectorList<SP<SomeUIEvent>> BuildUIEvents(SDL_Event sdlEvent);
    };
}

#endif
