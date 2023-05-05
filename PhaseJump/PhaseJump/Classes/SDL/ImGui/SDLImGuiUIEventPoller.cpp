#include "SDLImGuiUIEventPoller.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "SDLUIEventsBuilder.h"
#include "SomeUIEvent.h"

using namespace std;
using namespace PJ;

SDLImGuiUIEventPoller::SDLImGuiUIEventPoller(SDL_Window* window) : window(window) {
}

SomeUIEventPoller::Result SDLImGuiUIEventPoller::PollUIEvents() {
    VectorList<SP<SomeUIEvent>> uiEvents;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        // Let imGui steal mouse events for its windows
        bool canCreateUIEvent = true;
        auto imGuiWantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
        if (imGuiWantCaptureMouse) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP:
                    canCreateUIEvent = false;
                    break;
                default:
                    break;
            }
        }

        if (canCreateUIEvent) {
            VectorList<SP<SomeUIEvent>> thisUIEvents = SDLUIEventsBuilder().BuildUIEvents(event);
            uiEvents.AddRange(thisUIEvents);
        }

        if (event.type == SDL_QUIT
            || (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
             && event.window.windowID == SDL_GetWindowID(window))) {
            return Result(Status::Done, uiEvents);
        }
    }

    return Result(Status::Running, uiEvents);
}
