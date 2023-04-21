#include "SDLImGuiUIEventPoller.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "SDLUIEventBuilder.h"

using namespace std;
using namespace PJ;

SDLImGuiUIEventPoller::SDLImGuiUIEventPoller(SDL_Window* window) : window(window) {
}

SomeUIEventPoller::Result SDLImGuiUIEventPoller::PollUIEvents() {
    VectorList<std::shared_ptr<SomeUIEvent>> uiEvents;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        VectorList<std::shared_ptr<SomeUIEvent>> thisUIEvents = SDLUIEventBuilder().BuildUIEvents(event);
        uiEvents.AddRange(thisUIEvents);

        if (event.type == SDL_QUIT
            || (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
             && event.window.windowID == SDL_GetWindowID(window))) {
            return Result(Status::Done, uiEvents);
        }
    }

    return Result(Status::Running, uiEvents);
}
