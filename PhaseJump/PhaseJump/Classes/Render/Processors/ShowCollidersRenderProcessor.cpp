#include "ShowCollidersRenderProcessor.h"
#include "Colliders2D.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderWorldSystem.h"
#include "SomeCollider2D.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

void AddOverlay(
    RenderSystemModel& systemModel, WorldNode* node, SP<RenderMaterial> colliderMaterial,
    Color color, Vector2 overlaySize
) {
    ColorRenderer renderer(colliderMaterial, color, overlaySize);
    renderer.model.SetMeshBuilderFunc([=](RendererModel const& model) {
        // FUTURE: draw the actual polygon, not just the frame
        QuadFrameMeshBuilder builder(overlaySize, Vector2(3, 3));
        Mesh mesh = builder.BuildMesh();
        return mesh;
    });

    renderer.owner = node;
    auto debugRenderModels = renderer.MakeRenderModels();
    AddRange(systemModel.models, debugRenderModels);
}

void ShowCollidersRenderProcessor::Process(RenderSystemModel& systemModel) {
    auto colliderMaterial = ColorRenderer::MakeMaterial(
        color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend
    );
    systemModel.materials.push_back(colliderMaterial);

    // FUTURE: this can be optimized
    for (auto& node : systemModel.nodes) {
        auto colliders = node->GetComponents<SomeCollider2D>();
        for (auto& collider : colliders) {
            auto polyCollider = As<PolygonCollider2D>(collider.get());

            if (polyCollider) {
                Vector2 overlaySize(polyCollider->poly.Width(), polyCollider->poly.Height());
                AddOverlay(systemModel, node, colliderMaterial, color, overlaySize);
                continue;
            }

            auto circleCollider = As<CircleCollider2D>(collider.get());

            // FUTURE: support more colliders as needed
            if (circleCollider) {
                Vector2 overlaySize(circleCollider->radius * 2, circleCollider->radius * 2);
                AddOverlay(systemModel, node, colliderMaterial, color, overlaySize);
                continue;
            }
        }
    }
}
