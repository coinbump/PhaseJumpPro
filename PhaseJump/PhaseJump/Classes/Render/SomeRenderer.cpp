#include "SomeRenderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"

using namespace PJ;

SomeRenderer::SomeRenderer(Vector3 worldSize) :
    model(worldSize) {

    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<SomeTexture*> textures;
        VectorList<RenderModel> result;

        auto material = model.material;
        if (nullptr == material) {
            PJ::Log("ERROR. Missing material.");
            return result;
        }

        auto& materialTextures = material->Textures();

        RenderModelBuilder builder;
        auto renderModel = builder.Build(*this, model.Mesh(), *material, materialTextures);
        renderModel.SetVertexColors(std::span<RenderColor const>(model.VertexColors()));

        Add(result, renderModel);

        return result;
    });
}

VectorList<RenderModel> SomeRenderer::MakeRenderModels() {
    auto result = model.RenderModels();

    // Always update the matrix for latest transform
    for (auto& renderModel : result) {
        renderModel.matrix = ModelMatrix();
    }

    return result;
}
