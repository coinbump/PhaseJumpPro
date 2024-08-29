#include "ShowCollidersRenderProcessor.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "SomeCollider2D.h"
#include "QuadFrameMeshBuilder.h"
#include "Colliders2D.h"
#include "RenderSystem.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

void ShowCollidersRenderProcessor::Process(RenderSystemModel& systemModel) {
    auto colliderMaterial = ColorRenderer::MakeMaterial(color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend);
    systemModel.materials.push_back(colliderMaterial);

    // FUTURE: this can be optimized
    for (auto& node : systemModel.nodes) {
        auto colliders = node->GetComponents<SomeCollider2D>();
        for (auto& collider : colliders) {
            auto polyCollider = As<PolygonCollider2D>(collider.get());

            // FUTURE: support more colliders as needed
            if (polyCollider) {
                Vector2 polySize(polyCollider->poly.Width(), polyCollider->poly.Height());

                ColorRenderer renderer(colliderMaterial, color, polySize);
                renderer.model.SetMeshBuilderFunc([=](MeshRendererModel const& model) {
                    // FUTURE: draw the actual polygon, not just the frame
                    QuadFrameMeshBuilder builder(polySize, Vector2(3, 3));
                    Mesh mesh = builder.BuildMesh();
                    return mesh;
                });

                renderer.owner = node;
                auto debugRenderModels = renderer.MakeRenderModels();
                AddRange(systemModel.models, debugRenderModels);
            }
        }
    }
}
