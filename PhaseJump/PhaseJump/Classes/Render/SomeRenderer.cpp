#include "SomeRenderer.h"
#include "RenderIntoModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace PJ;

void ActionRenderer::RenderInto(RenderIntoModel const& model) {
    render(model);
}

VectorList<RenderModel> SomeRenderer::MakeRenderModels(
    RenderIntoModel const& model, Mesh const& mesh, VectorList<SomeTexture*> textures
) {
    VectorList<RenderModel> result;
    GUARDR(owner, result)

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(),
        0 // TODO: rethink this: owner->transform->WorldPosition().z
    );

    result.Add(renderModel);
    return result;
}
