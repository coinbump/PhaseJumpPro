#include "World.h"
#include "SomeRenderer.h"
#include "RenderIntoModel.h"
#include <stdio.h>

using namespace PJ;

void World::Render()
{
    if (!renderContext) { return; }

    renderContext->Bind();
    renderContext->Clear();

    auto graph = root->CollectGraph();
    for (auto node : graph) {
        auto worldNode = std::dynamic_pointer_cast<WorldNode>(node);
        for (auto component : worldNode->Components()) {
            auto renderer = std::dynamic_pointer_cast<SomeRenderer>(component);
            if (!renderer) { continue; }

            RenderIntoModel model;
            model.renderContext = renderContext;

            auto localPosition = worldNode->transform->position;
            auto worldPosition = localPosition;
            auto parent = static_pointer_cast<WorldNode>(worldNode->Parent());

            // Transform local position to world position
            while (parent) {
                auto parentPosition = parent->transform->position;
                worldPosition += parentPosition;

                parent = static_pointer_cast<WorldNode>(parent->Parent());
            }

            model.position = camera->WorldToScreen(worldPosition, *renderContext);

            renderContext->Render(*renderer, model);
        }
    }

    renderContext->Present();
}
