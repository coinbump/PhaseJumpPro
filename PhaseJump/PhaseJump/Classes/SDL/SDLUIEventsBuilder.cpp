#include "SDLUIEventsBuilder.h"
#include "FilePath.h"
#include "SomeUIEvent.h"

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

VectorList<SP<SomeUIEvent>> SDLUIEventsBuilder::BuildUIEvents(SDL_Event sdlEvent) {
    VectorList<SP<SomeUIEvent>> result;

    switch (sdlEvent.type) {
        case SDL_DROPFILE: {
            auto droppedFile = sdlEvent.drop.file;
            auto event = MAKE<DropFileUIEvent>(FilePath(droppedFile));
            result.Add(SCAST<SomeUIEvent>(event));
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

    return result;
}
