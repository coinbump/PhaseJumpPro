#include "SDLPlatformWindow.h"
#include <SDL3/SDL.h>

using namespace PJ;

void SDLPlatformWindow::Configure(Config& config) {
    switch (config.rendererType) {
    // Create window and renderer (easy mode)
    case Config::RendererType::WindowAndRenderer:
        if (!SDL_CreateWindowAndRenderer(
                config.title.c_str(), config.size.x, config.size.y, 0, &window, &renderer
            )) {
            std::cout << "ERROR: Can't create SDL Window.";
            return;
        }
        break;
    // Create window with a custom render context (Example: OpenGL context).
    // Requires custom rendering
    case Config::RendererType::Window:
        window = SDL_CreateWindow(
            config.title.c_str(), config.size.x, config.size.y, config.windowFlags
        );

        if (nullptr == window) {
            std::cout << "ERROR: Can't create SDL Window.";
            return;
        }
        break;
    }

    GUARD(world)
    world->Configure(window, nullptr);

    // Important: Call world->Go after you've built your scene
}
