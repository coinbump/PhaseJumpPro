#include "SDLImGuiUIEventPoller.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "List.h"
#include "SDLEventPoller.h"
#include "SDLUIEventsBuilder.h"
#include "SomeUIEvent.h"

using namespace std;
using namespace PJ;

SDLImGuiUIEventPoller::SDLImGuiUIEventPoller(SDL_Window* window) :
    window(window) {}

SomeUIEventPoller::Result SDLImGuiUIEventPoller::PollUIEvents() {
    VectorList<SP<SomeUIEvent>> uiEvents;

    SDL_Event event;
    SDLEventList events;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);

        // Let imGui steal mouse events for its windows
        auto imGuiWantCaptureMouse = ImGui::GetIO().WantCaptureMouse;
        if (imGuiWantCaptureMouse) {
            switch (event.type) {
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_MOTION:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                continue;
                break;
            default:
                break;
            }
        }

        Add(events, event);
    }

    return SDLEventPoller(window).Process(events);
}
