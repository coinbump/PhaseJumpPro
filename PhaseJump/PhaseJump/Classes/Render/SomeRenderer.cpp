#include "SomeRenderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace PJ;

VectorList<RenderModel>
SomeRenderer::MakeRenderModels(Mesh const& mesh, VectorList<SomeTexture*> textures) {
    VectorList<RenderModel> result;
    GUARDR(owner, result)
    GUARDR_LOG(model.material, result, String("ERROR. Missing material"))

    RenderModelBuilder builder;
    auto renderModel = builder.Build(*this, mesh, *model.material, textures);

    Add(result, renderModel);
    return result;
}
