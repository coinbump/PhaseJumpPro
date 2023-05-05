#include "SomeCamera.h"
#include "RenderIntoModel.h"
#include "SomeRenderer.h"
#include "SomeRenderContext.h"
#include "Matrix4x4.h"

using namespace std;
using namespace PJ;

void SomeCamera::Render(VectorList<SP<WorldNode>> nodes, SP<SomeRenderContext> renderContext)
{
    for (auto node : nodes) {
        for (auto component : node->Components()) {
            auto renderer = DCAST<SomeRenderer>(component);
            if (!renderer) { continue; }

            RenderIntoModel model;
            model.renderContext = renderContext;

            auto owner = node;

            auto worldModelMatrix = WorldModelMatrix(*node);
            model.modelMatrix = worldModelMatrix;

            renderContext->Render(*renderer, model);
        }
    }
}

Matrix4x4 SomeCamera::LocalModelMatrix(WorldNode const& node) {
    Matrix4x4 translateMatrix, scaleMatrix, rotationMatrix;
    translateMatrix.LoadTranslate(node.transform->position);
    scaleMatrix.LoadScale(node.transform->scale);

    // This is 2D rotation only
    // FUTURE: support 3D rotation if needed.
    rotationMatrix.LoadZRadRotation(Angle::DegreesAngle(node.transform->rotation.z).Radians());

    auto m1 = translateMatrix * rotationMatrix;
    auto modelMatrix = m1 * scaleMatrix;
    return modelMatrix;
}

Matrix4x4 SomeCamera::WorldModelMatrix(WorldNode const& node) {
    auto modelMatrix = LocalModelMatrix(node);

    auto parent = SCAST<WorldNode>(node.Parent());

    // Transform child to parent matrix
    while (parent) {
        auto parentModelMatrix = LocalModelMatrix(*parent);

        modelMatrix = parentModelMatrix * modelMatrix;

        parent = SCAST<WorldNode>(parent->Parent());
    }

    Matrix4x4 worldScaleMatrix;
    worldScaleMatrix.LoadIdentity();

    modelMatrix = worldScaleMatrix * modelMatrix;

    return modelMatrix;
}
