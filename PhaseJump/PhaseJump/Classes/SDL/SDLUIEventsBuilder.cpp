#include "SDLUIEventsBuilder.h"
#include "FilePath.h"
#include "SomeUIEvent.h"

using namespace std;
using namespace PJ;

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
            auto inputButton = SomePointerUIEvent::InputButton::Left;
            switch (sdlEvent.button.button) {
                case SDL_BUTTON_RIGHT:
                    inputButton = SomePointerUIEvent::InputButton::Right;
                    break;
                case SDL_BUTTON_MIDDLE:
                    inputButton = SomePointerUIEvent::InputButton::Middle;
                    break;
                default:
                    break;
            }

            auto screenPosition = ScreenPosition(Vector2(sdlEvent.button.x, sdlEvent.button.y));
            auto event = MAKE<PointerDownUIEvent<ScreenPosition>>(screenPosition, inputButton);
            result.Add(SCAST<SomeUIEvent>(event));
        }
    }

    return result;
}
