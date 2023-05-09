#include "SDLMouseDevice.h"
#include <SDL2/SDL.h>

using namespace std;
using namespace PJ;

ScreenPosition SDLMouseDevice::Position() const {
    int x, y;
    SDL_GetMouseState(&x, &y);

    return ScreenPosition(Vector2(x, y));
}
