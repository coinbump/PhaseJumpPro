#include "ShowBoundsRenderProcessor.h"
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

void ShowBoundsRenderProcessor::Process(CameraRenderModel& cameraModel) {
    auto boundsMaterial = ColorRenderer::MakeMaterial(RenderOpacityTypeFor(color));
    cameraModel.materials.push_back(boundsMaterial);

    cameraModel.models.reserve(cameraModel.models.size() + cameraModel.nodes.size());

    // FUTURE: this can be optimized
    for (auto& node : cameraModel.nodes) {
        auto& components = node->Components();
        for (auto& component : components) {
            auto ws = As<WorldSizeable>(component.get());
            GUARD_CONTINUE(ws)

            auto worldSize = ws->WorldSize();

            auto renderer = MAKE<ColorRenderer>(ColorRenderer::Config{
                .material = boundsMaterial, .color = color, .worldSize = worldSize });
            renderer->model.SetBuildMeshFunc([=](RendererModel const& model) {
                // TODO: mesh.Resize(model.WorldSize());
                QuadFrameMeshBuilder builder(model.WorldSize(), Vector2(2, 2));
                Mesh mesh = builder.BuildMesh();
                return mesh;
            });
            cameraModel.renderers.push_back(renderer);

            renderer->owner = node;
            auto debugRenderModels = renderer->RenderModels();
            AddRange(cameraModel.models, debugRenderModels);
            break;
        }
    }
}
