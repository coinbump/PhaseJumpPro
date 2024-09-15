#include "SDLPlatformWindow.h"
#include <SDL3/SDL.h>

using namespace PJ;

SDLPlatformWindow::Configuration const SDLPlatformWindow::Configuration::native(
    Vector2Int(100, 100), 0, RendererType::WindowAndRenderer
);
SDLPlatformWindow::Configuration const SDLPlatformWindow::Configuration::openGL(
    Vector2Int(100, 100), SDL_WINDOW_OPENGL, RendererType::Window
);

void SDLPlatformWindow::GoInternal() {
    Base::GoInternal();

    switch (config.rendererType) {
    // Create window and renderer (easy mode)
    case Configuration::RendererType::WindowAndRenderer:
        if (SDL_CreateWindowAndRenderer("", config.size.x, config.size.y, 0, &window, &renderer) <
            0) {
            std::cout << "ERROR: Can't create SDL Window.";
            return;
        }
        break;
    // Create window with a custom render context (Example: OpenGL context).
    // Requires custom rendering
    case Configuration::RendererType::Window:
        window = SDL_CreateWindow("Phase Jump", config.size.x, config.size.y, config.windowFlags);

        if (nullptr == window) {
            std::cout << "ERROR: Can't create SDL Window.";
            return;
        }
        break;
    }

    GUARD(world)
    world->Configure(window, nullptr);

    // world->Configure(window, MAKE<SDLRenderContext>(renderer));

    // IMPORTANT: Call world->Go after you've built your scene
}
