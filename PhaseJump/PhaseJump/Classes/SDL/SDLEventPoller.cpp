#include "SDLEventPoller.h"

using namespace PJ;

SDLEventPoller::Result SDLEventPoller::PollUIEvents() {
    SDL_Event event;
    SDLEventList events;
    while (SDL_PollEvent(&event)) {
        events.Add(event);
    }

    return Process(events);
}

SDLEventPoller::Result SDLEventPoller::Process(SDLEventList const& events) {
    List<SP<SomeUIEvent>> uiEvents;

    // Events need to be processed together because some events are combined
    // (multiple Drop events for multiple files dropped simultaneously)
    for (auto& event : events) {
        if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                                             event.window.windowID == SDL_GetWindowID(window))) {
            return Result(Status::Done, uiEvents);
        }
    }

    List<SP<SomeUIEvent>> thisUIEvents = SDLUIEventsBuilder().BuildUIEvents(events);
    AddRange(uiEvents, thisUIEvents);

    return Result(Status::Running, uiEvents);
}
