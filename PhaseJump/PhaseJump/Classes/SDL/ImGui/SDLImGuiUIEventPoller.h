#ifndef PJSDLIMGUIUIEVENTPOLLER_H
#define PJSDLIMGUIUIEVENTPOLLER_H

#include "SomeUIEventPoller.h"
#include "VectorList.h"
#include "SomePosition.h"
#include "FilePath.h"
#include <SDL2/SDL.h>

class SDL_Window;

namespace PJ {
    /// Polls SDL events and forwards them to Dear ImGui
    class SDLImGuiUIEventPoller : public SomeUIEventPoller {
    public:
        SDL_Window* window;

        SDLImGuiUIEventPoller(SDL_Window* window);

        Result PollUIEvents() override;
    };
};

#endif
