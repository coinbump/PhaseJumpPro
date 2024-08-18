#ifndef PJSDLIMGUIUIEVENTPOLLER_H
#define PJSDLIMGUIUIEVENTPOLLER_H

#include "FilePath.h"
#include "SomePosition.h"
#include "SomeUIEventPoller.h"
#include "VectorList.h"
#include <SDL3/SDL.h>

class SDL_Window;

// CODE REVIEW: ?/23
namespace PJ {
    /// Polls SDL events and forwards them to Dear ImGui
    class SDLImGuiUIEventPoller : public SomeUIEventPoller {
    public:
        SDL_Window* window;

        SDLImGuiUIEventPoller(SDL_Window* window);

        virtual ~SDLImGuiUIEventPoller() {}

        Result PollUIEvents() override;
    };
}; // namespace PJ

#endif
