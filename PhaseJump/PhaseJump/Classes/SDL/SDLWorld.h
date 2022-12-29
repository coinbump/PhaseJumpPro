#ifndef PJSDLWORLD_H
#define PJSDLWORLD_H

#include "World.h"
#include "SomeRenderer.h"
#include "SDLTextureRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <memory>

namespace PJ {
    class SDLWorld : public World {
    public:
        using Base = World;

    protected:
        bool isDone = false;

        void GoInternal() override {
            Base::GoInternal();

            while (!isDone) {
                mainLoop();
            }
            SDL_Quit();
        }

    public:
        SDLWorld(std::shared_ptr<SomeRenderContext> renderContext) {
            this->renderContext = renderContext;
        }

        void mainLoop()
        {
            if (!renderContext) { return; }

            // TODO: temp code
            // TODO: dispatch to event system
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
                    isDone = true;
                }
            }

            renderContext->Bind();
            renderContext->Clear();

            auto graph = root->CollectGraph();
            for (auto node : graph) {
                auto worldNode = std::dynamic_pointer_cast<WorldNode>(node);
                for (auto component : worldNode->Components()) {
                    auto renderer = std::dynamic_pointer_cast<SomeRenderer>(component);
                    if (!renderer) { continue; }
                    renderer->RenderInto(renderContext);
                }
            }

            renderContext->Present();
        }
    };
}

#endif
