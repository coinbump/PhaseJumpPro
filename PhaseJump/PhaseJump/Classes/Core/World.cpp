#include "World.h"
#include "SomeRenderer.h"
#include "RenderIntoModel.h"
#include "SomeCamera.h"
#include "SomeRenderEngine.h"
#include <stdio.h>

using namespace std;
using namespace PJ;

void World::Render()
{
    if (!renderContext) { return; }

    renderContext->Bind();
    renderContext->Clear();

    auto graph = root->CollectBreadthFirstGraph();
    VectorList<std::shared_ptr<WorldNode>> nodes;
    VectorList<std::shared_ptr<WorldNode>> activeNodes;

    std::transform(graph.begin(), graph.end(), std::back_inserter(nodes), [](std::shared_ptr<SomeGraphNode<>> node) {
        return static_pointer_cast<WorldNode>(node);
    });

    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(activeNodes), [](std::shared_ptr<WorldNode> node) {
        return node->IsActive();
    });

    auto renderEngine = renderContext->renderEngine;
    renderEngine->RenderStart();

    for (auto node : activeNodes) {
        auto camera = node->GetComponent<SomeCamera>();
        if (nullptr == camera) {
            continue;
        }

        camera->Render(activeNodes, renderContext);
    }

    renderEngine->RenderDraw();

    renderContext->Present();
}
