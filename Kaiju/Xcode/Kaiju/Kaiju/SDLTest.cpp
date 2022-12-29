#ifndef _TESTS_

#include "SDLTest.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <PhaseJump/SDLRenderContext.h>
#include <PhaseJump/SDLWindow.h>
#include <PhaseJump/SDLWorld.h>
#include <PhaseJump/HFlow.h>
#include <PhaseJump/VFlow.h>
#include <PhaseJump/StandardNormalRandom.h>
#include <iostream>

using namespace PJ;
using namespace std;

void SDLFoo() {
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    auto window = std::make_shared<SDLWindow>(Vector2Int(640, 480));
    window->Go();

    static std::shared_ptr<SDLTexture> sprite;
    if (!sprite) {
        SDLLoadTextureOperation loadTexture(window->Renderer(), "resources/heart-full.png");
        loadTexture.Go();
        sprite = loadTexture.texture;
    }

    if (!sprite) {
        std::cout << "ERROR: Missing sprite.";
        return;
    }

    for (int i = 0; i < 20; i++) {
        auto node = std::make_shared<WorldNode>();
        window->World()->root->AddEdge(StandardEdgeModel(), node);
        auto renderer = std::make_shared<SDLTextureRenderer>(sprite);
        node->AddComponent(renderer);
        node->transform->position = Vector3(StandardNormalRandom().Value() * 100.0f, StandardNormalRandom().Value() * 100.0f, 0);
    }

    auto layout = make_shared<VFlow>(20);
    window->World()->root->AddComponent(layout);

    // This won't work correctly until we support center-out coordinates
//    layout->ApplyLayout();

    window->World()->Go();
}

#endif
