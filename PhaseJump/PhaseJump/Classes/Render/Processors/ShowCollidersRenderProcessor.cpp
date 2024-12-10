#include "ShowCollidersRenderProcessor.h"
#include "Colliders2D.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderWorldSystem.h"
#include "SomeCollider2D.h"
#include "SomeRenderEngine.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void AddOverlay(
    CameraRenderModel& cameraModel, WorldNode* node, SP<RenderMaterial> colliderMaterial,
    Color color, Vector2 overlaySize
) {
    auto renderer = MAKE<ColorRenderer>(ColorRenderer::Config{
        .material = colliderMaterial, .color = color, .worldSize = overlaySize });
    renderer->model.SetBuildMeshFunc([=](RendererModel const& model) {
        // FUTURE: draw the actual polygon, not just the frame
        QuadFrameMeshBuilder builder(overlaySize, Vector2(3, 3));
        Mesh mesh = builder.BuildMesh();
        return mesh;
    });
    cameraModel.renderers.push_back(renderer);

    renderer->owner = node;
    auto debugRenderModels = renderer->RenderModels();
    AddRange(cameraModel.models, debugRenderModels);
}

void ShowCollidersRenderProcessor::Process(CameraRenderModel& cameraModel) {
    auto colliderMaterial = ColorRenderer::MakeMaterial(RenderOpacityTypeFor(color));
    cameraModel.materials.push_back(colliderMaterial);

    // FUTURE: this can be optimized
    for (auto& node : cameraModel.nodes) {
        auto colliders = node->GetComponents<SomeCollider2D>();
        for (auto& collider : colliders) {
            auto polyCollider = As<PolygonCollider2D>(collider);

            if (polyCollider) {
                Vector2 overlaySize(polyCollider->poly.Width(), polyCollider->poly.Height());
                AddOverlay(cameraModel, node, colliderMaterial, color, overlaySize);
                continue;
            }

            auto circleCollider = As<CircleCollider2D>(collider);

            // FUTURE: support more colliders as needed
            if (circleCollider) {
                Vector2 overlaySize(circleCollider->radius * 2, circleCollider->radius * 2);
                AddOverlay(cameraModel, node, colliderMaterial, color, overlaySize);
                continue;
            }
        }
    }
}
