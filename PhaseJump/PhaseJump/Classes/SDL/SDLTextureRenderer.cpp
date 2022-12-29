#include "SDLTextureRenderer.h"
#include <SDL2/SDL_main.h>

using namespace PJ;

SDL_Vertex SDLTextureRenderer::newVertex(Mesh mesh, Vector2 position, int index) {
    SDL_Vertex result;
    result.position.x = position.x();
    result.position.y = position.y();

    if (mesh.colors.IsEmpty()) {
        result.color = SDLColorBuilder().Transform(Color::white);
    }
    else {
        auto colorIndex = index % mesh.colors.Count();
        result.color = SDLColorBuilder().Transform(mesh.colors[colorIndex]);
    }

    return result;
}
