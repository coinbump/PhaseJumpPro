#include "SDLUIEventPoller.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "List.h"
#include "SDLUIEventsBuilder.h"
#include "SomeUIEvent.h"

using namespace std;
using namespace PJ;

SDLUIEventPoller::SDLUIEventPoller(SDL_Window* window) :
    window(window) {}

SomeUIEventPoller::Result SDLUIEventPoller::PollUIEvents() {
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

    return Process(events);
}

SDLUIEventPoller::Result SDLUIEventPoller::Process(SDLEventList const& events) {
    UIEventList uiEvents;

    // Events should be processed together because some events are combined
    // (multiple Drop events for multiple files dropped simultaneously)
    for (auto& event : events) {
        if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                                             event.window.windowID == SDL_GetWindowID(window))) {
            return Result(StateType::Finished, uiEvents);
        }
    }

    float uiScale = SDL_GetWindowPixelDensity(window);

    UIEventList thisUIEvents = SDLUIEventsBuilder().BuildUIEvents(events, uiScale);
    AddRange(uiEvents, thisUIEvents);

    return Result(StateType::Run, uiEvents);
}
