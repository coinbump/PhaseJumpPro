#include "ShowBoundsRenderProcessor.h"
#include "Colliders2D.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderWorldSystem.h"
#include "SomeCollider2D.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

void ShowBoundsRenderProcessor::Process(RenderSystemModel& systemModel) {
    auto boundsMaterial = ColorRenderer::MakeMaterial(
        color.IsOpaque() ? RenderOpacityType::Opaque : RenderOpacityType::Blend
    );
    systemModel.materials.push_back(boundsMaterial);

    systemModel.models.reserve(systemModel.models.size() + systemModel.nodes.size());

    // FUTURE: this can be optimized
    for (auto& node : systemModel.nodes) {
        auto& components = node->Components();
        for (auto& component : components) {
            auto ws = As<WorldSizeable>(component.get());
            GUARD_CONTINUE(ws)

            auto worldSize = ws->WorldSize();
            GUARD_CONTINUE(worldSize)

            ColorRenderer renderer(boundsMaterial, color, *worldSize);
            renderer.model.SetMeshBuilderFunc([=](RendererModel const& model) {
                // TODO: mesh.Resize(model.WorldSize());
                QuadFrameMeshBuilder builder(model.WorldSize(), Vector2(2, 2));
                Mesh mesh = builder.BuildMesh();
                return mesh;
            });

            renderer.owner = node;
            auto debugRenderModels = renderer.MakeRenderModels();
            AddRange(systemModel.models, debugRenderModels);
            break;
        }
    }
}
