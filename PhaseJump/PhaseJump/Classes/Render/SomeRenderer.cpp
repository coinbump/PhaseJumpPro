#include "SomeRenderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace PJ;

void ActionRenderer::RenderInto(RenderContextModel const& contextModel) {
    render(contextModel);
}

VectorList<RenderModel>
SomeRenderer::MakeRenderModels(Mesh const& mesh, VectorList<SomeTexture*> textures) {
    VectorList<RenderModel> result;

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(),
        0 // TODO: rethink this: owner->transform.WorldPosition().z
    );

    Add(result, renderModel);
    return result;
}
