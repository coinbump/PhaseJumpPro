#include "SDLWindow.h"

using namespace PJ;

SDLWindow::Configuration const SDLWindow::Configuration::native(0, RendererType::WindowAndRenderer);
SDLWindow::Configuration const SDLWindow::Configuration::openGL(SDL_WINDOW_OPENGL, RendererType::Window);

void SDLWindow::GoInternal() {
    Base::GoInternal();

    switch (configuration.rendererType) {
        // Create window and renderer (easy mode)
        case Configuration::RendererType::WindowAndRenderer:
            if (SDL_CreateWindowAndRenderer(size.x, size.y, 0, &window, &renderer) < 0) {
                std::cout << "ERROR: Can't create SDL Window.";
                return;
            }
            break;
        // Create window with a custom render context (Example: OpenGL context). Requires custom rendering
        case Configuration::RendererType::Window:
            window = SDL_CreateWindow("Phase Jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, configuration.windowFlags);

            if (nullptr == window) {
                std::cout << "ERROR: Can't create SDL Window.";
                return;
            }
            break;
    }

    world = std::make_shared<SDLWorld>(std::make_shared<SDLRenderContext>(renderer));

    // IMPORTANT: Call world->Go after you've built your scene
}
