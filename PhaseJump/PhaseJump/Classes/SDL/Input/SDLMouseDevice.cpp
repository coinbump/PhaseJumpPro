#include "SDLMouseDevice.h"
#include "PointerUIEventTypes.h"
#include <SDL3/SDL.h>

using namespace std;
using namespace PJ;

ScreenPosition SDLMouseDevice::GetScreenPosition() const {
    float x, y;
    SDL_GetMouseState(&x, &y);

    return { x, y };
}

bool SDLMouseDevice::IsButtonDown(PointerInputButtonType button) {
    uint32_t buttonMask = 0;

    if (button == PointerInputButton::Left) {
        buttonMask = SDL_BUTTON_LMASK;
    } else if (button == PointerInputButton::Middle) {
        buttonMask = SDL_BUTTON_MMASK;
    } else if (button == PointerInputButton::Right) {
        buttonMask = SDL_BUTTON_RMASK;
    }

    auto result = (SDL_GetMouseState(NULL, NULL) & buttonMask) != 0;
    return result;
}
