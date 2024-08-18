#include "MeshRenderer.h"
#include "Angle.h"
#include "RenderIntoModel.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace std;
using namespace PJ;

VectorList<RenderModel> MeshRenderer::MakeRenderModels(RenderIntoModel const& model) {
    VectorList<RenderModel> result;
    GUARDR(owner, result)

    if (nullptr == material) {
        PJLog("ERROR. Missing material.");
        return result;
    }

    RenderModelBuilder builder;
    VectorList<SomeTexture*> textures;
    auto renderModel = builder.Build(
        mesh, *material, textures, ModelMatrix(), owner->transform->WorldPosition().z
    );

    result.Add(renderModel);
    return result;
}
