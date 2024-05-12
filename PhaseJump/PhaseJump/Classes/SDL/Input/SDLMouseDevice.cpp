#include "SDLMouseDevice.h"
#include "SomePointerUIEvent.h"
#include <SDL2/SDL.h>

using namespace std;
using namespace PJ;

ScreenPosition SDLMouseDevice::Position() const {
    int x, y;
    SDL_GetMouseState(&x, &y);

    return ScreenPosition(Vector2(x, y));
}

bool SDLMouseDevice::IsButtonDown(PointerInputButton button) {
    uint32_t buttonMask = 0;

    switch (button) {
        case PointerInputButton::Left:
            buttonMask = SDL_BUTTON_LMASK;
            break;
        case PointerInputButton::Middle:
            buttonMask = SDL_BUTTON_MMASK;
            break;
        case PointerInputButton::Right:
            buttonMask = SDL_BUTTON_RMASK;
            break;
        default:
            break;
    }

    auto result = (SDL_GetMouseState(NULL, NULL) & buttonMask) != 0;
    return result;
}
