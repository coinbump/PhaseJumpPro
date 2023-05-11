#include "SomeCamera.h"
#include "RenderIntoModel.h"
#include "SomeRenderer.h"
#include "SomeRenderContext.h"
#include "Matrix4x4.h"
#include "World.h"

using namespace std;
using namespace PJ;

void SomeCamera::Render(VectorList<SP<WorldNode>> nodes, SP<SomeRenderContext> renderContext)
{
    auto world = World();

    for (auto node : nodes) {
        for (auto component : node->Components()) {
            auto renderer = DCAST<SomeRenderer>(component);
            if (!renderer) { continue; }

            RenderIntoModel model;
            model.renderContext = renderContext;

            auto owner = node;

            auto worldModelMatrix = world->WorldModelMatrix(*node);
            model.modelMatrix = worldModelMatrix;

            renderContext->Render(*renderer, model);
        }
    }
}
