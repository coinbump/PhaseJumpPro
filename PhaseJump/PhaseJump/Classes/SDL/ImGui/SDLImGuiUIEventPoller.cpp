#include "SDLImGuiUIEventPoller.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "SDLUIEventsBuilder.h"
#include "SomeUIEvent.h"
#include "SDLEventPoller.h"
#include "List.h"

using namespace std;
using namespace PJ;

SDLImGuiUIEventPoller::SDLImGuiUIEventPoller(SDL_Window* window) : window(window) {
}

SomeUIEventPoller::Result SDLImGuiUIEventPoller::PollUIEvents() {
    VectorList<SP<SomeUIEvent>> uiEvents;

    SDL_Event event;
    SDLEventList events;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        // Let imGui steal mouse events for its windows
        auto imGuiWantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
        if (imGuiWantCaptureMouse) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONUP:
                    continue;
                    break;
                default:
                    break;
            }
        }

        events.Add(event);
    }

    return SDLEventPoller(window).Process(events);
}
