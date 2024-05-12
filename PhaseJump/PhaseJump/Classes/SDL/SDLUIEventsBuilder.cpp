#include "SDLUIEventsBuilder.h"
#include "FilePath.h"
#include "PointerClickUIEvent.h"

using namespace std;
using namespace PJ;

PointerInputButton PointerInputButtonFromSDLButton(Uint8 sdlButton) {
    auto inputButton = SomePointerUIEvent::InputButton::Left;
    switch (sdlButton) {
        case SDL_BUTTON_RIGHT:
            inputButton = SomePointerUIEvent::InputButton::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            inputButton = SomePointerUIEvent::InputButton::Middle;
            break;
        default:
            break;
    }

    return inputButton;
}

VectorList<SP<SomeUIEvent>> SDLUIEventsBuilder::BuildUIEvents(SDLEventList const& events) {
    VectorList<SP<SomeUIEvent>> result;

    VectorList<SP<DropFilesUIEvent>> dropFileEvents;

    for (auto sdlEvent : events) {
        switch (sdlEvent.type) {
            case SDL_DROPFILE: {
                // Combine separate drop file events
                auto droppedFile = sdlEvent.drop.file;
                auto event = MAKE<DropFilesUIEvent>(List<FilePath>{FilePath(droppedFile)});
                dropFileEvents.Add(event);
                SDL_free(droppedFile);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                auto screenPosition = ScreenPosition(Vector2(sdlEvent.button.x, sdlEvent.button.y));
                auto event = MAKE<PointerDownUIEvent<ScreenPosition>>(screenPosition, inputButton);
                result.Add(SCAST<SomeUIEvent>(event));
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                auto inputButton = PointerInputButtonFromSDLButton(sdlEvent.button.button);
                result.Add(MAKE<PointerUpUIEvent>(inputButton));
                break;
            }
            case SDL_MOUSEMOTION: {
                auto screenPosition = ScreenPosition(Vector2(sdlEvent.motion.x, sdlEvent.motion.y));
                auto delta = Vector2(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
                result.Add(MAKE<MouseMotionUIEvent<ScreenPosition>>(screenPosition, delta));
                break;
            }
        }
    }

    if (!dropFileEvents.IsEmpty()) {
        List<FilePath> combinedFilePaths;
        for (auto dropFileEvent : dropFileEvents) {
            combinedFilePaths.AddRange(dropFileEvent->filePaths);
        }
        auto event = MAKE<DropFilesUIEvent>(combinedFilePaths);
        result.Add(SCAST<SomeUIEvent>(event));
    }

    return result;
}
