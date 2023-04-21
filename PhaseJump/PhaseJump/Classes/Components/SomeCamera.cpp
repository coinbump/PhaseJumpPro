#include "SomeCamera.h"
#include "RenderIntoModel.h"
#include "SomeRenderer.h"
#include "SomeRenderContext.h"
#include "Matrix4x4.h"

using namespace std;
using namespace PJ;

void SomeCamera::Render(VectorList<std::shared_ptr<WorldNode>> nodes, std::shared_ptr<SomeRenderContext> renderContext)
{
    for (auto node : nodes) {
        for (auto component : node->Components()) {
            auto renderer = std::dynamic_pointer_cast<SomeRenderer>(component);
            if (!renderer) { continue; }

            RenderIntoModel model;
            model.renderContext = renderContext;

            auto parent = static_pointer_cast<WorldNode>(node->Parent());
            auto owner = node;

            auto modelMatrix = ModelMatrixFor(*node);

            // Transform child to parent matrix
            while (parent) {
                auto parentModelMatrix = ModelMatrixFor(*parent);

                modelMatrix = parentModelMatrix * modelMatrix;

                parent = static_pointer_cast<WorldNode>(parent->Parent());
            }

            Matrix4x4 worldScaleMatrix;
            worldScaleMatrix.LoadScale(Vector3((1.0f / pixelsPerUnit.x) * scale,
                                               (1.0f / pixelsPerUnit.y) * scale,
                                               (1.0f / pixelsPerUnit.z) * scale));

            modelMatrix = worldScaleMatrix * modelMatrix;

            model.modelMatrix = modelMatrix;

            renderContext->Render(*renderer, model);
        }
    }
}

Matrix4x4 SomeCamera::ModelMatrixFor(WorldNode const& node) {
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
