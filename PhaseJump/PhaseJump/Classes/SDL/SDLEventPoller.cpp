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
    VectorList<SP<SomeUIEvent>> uiEvents;

    // Events need to be processed together because some events are combined (multiple Drop events for multiple files dropped simultaneously)
    for (auto event : events) {
        if (event.type == SDL_QUIT
            || (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(window))) {
            return Result(Status::Done, uiEvents);
        }
    }

    VectorList<SP<SomeUIEvent>> thisUIEvents = SDLUIEventsBuilder().BuildUIEvents(events);
    uiEvents.AddRange(thisUIEvents);

    return Result(Status::Running, uiEvents);
}
