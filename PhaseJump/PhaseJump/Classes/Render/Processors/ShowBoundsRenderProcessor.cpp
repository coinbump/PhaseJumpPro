#include "ShowBoundsRenderProcessor.h"
#include "Collider2D.h"
#include "Colliders2D.h"
#include "Color.h"
#include "ColorRenderer.h"
#include "QuadFrameMeshBuilder.h"
#include "RenderWorldSystem.h"
#include "SomeRenderEngine.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

void ShowBoundsRenderProcessor::Process(RenderCameraModel& cameraModel) {
    auto boundsMaterial = ColorRenderer::MakeMaterial(RenderOpacityTypeFor(color));
    cameraModel.materials.push_back(boundsMaterial);

    // TODO: this is broken with new render DAG. Fix this
    /*
    cameraModel.renderModels.reserve(cameraModel.renderModels.size() + cameraModel.nodes.size());

    // FUTURE: this can be optimized
    for (auto& node : cameraModel.nodes) {
        auto& components = node->Components();
        for (auto& component : components) {
            auto ws = As<WorldSizeable>(component.get());
            GUARD_CONTINUE(ws)

            auto worldSize = ws->WorldSize();

            auto renderer = MAKE<ColorRenderer>(ColorRenderer::Config{
                .material = boundsMaterial, .color = color, .worldSize = worldSize });
            renderer->core.model.SetBuildMeshFunc([=](RendererModel const& model) {
                QuadFrameMeshBuilder builder(model.WorldSize(), Vector2(2, 2));
                Mesh mesh = builder.BuildMesh();
                return mesh;
            });
            cameraModel.renderers.push_back(renderer);

            renderer->owner = node;
            for (auto& sp : renderer->RenderModels()) {
                if (auto* material = dynamic_cast<MaterialRenderModel*>(sp.get())) {
                    cameraModel.renderModels.push_back(*material);
                }
            }
            break;
        }
    }
     */
}
