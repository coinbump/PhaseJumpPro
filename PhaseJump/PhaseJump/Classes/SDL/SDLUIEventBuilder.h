#ifndef PJSDLUIEVENTBUILDER_H
#define PJSDLUIEVENTBUILDER_H

#include "VectorList.h"
#include <memory>
#include <SDL2/SDL.h>

namespace PJ {
    class SomeUIEvent;

    struct SDLUIEventBuilder {
        VectorList<std::shared_ptr<SomeUIEvent>> BuildUIEvents(SDL_Event sdlEvent);
    };
}

#endif
