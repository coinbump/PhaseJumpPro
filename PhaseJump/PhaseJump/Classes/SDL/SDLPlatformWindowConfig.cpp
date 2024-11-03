#include "SDLPlatformWindowConfig.h"

using namespace std;
using namespace PJ;

// FUTURE: SDLPlatformWindow::Config const
//    SDLPlatformWindow::Config::native(Vector2Int(100, 100), 0, RendererType::WindowAndRenderer);
SDLPlatformWindowConfig const
    SDLPlatformWindowConfig::openGL({ .windowFlags = SDL_WINDOW_OPENGL,
                                      .rendererType = RendererType::Window });
