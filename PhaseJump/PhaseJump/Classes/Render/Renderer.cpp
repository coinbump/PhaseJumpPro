#include "Renderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace PJ;

MaterialRenderer::MaterialRenderer(Vector3 worldSize) :
    model(worldSize) {

    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<Texture*> textures;
        VectorList<RenderModel> result;

        RenderModelBuilder builder;
        auto renderModel = builder.Build(*this, model);
        GUARDR(renderModel, result)
        renderModel->SetVertexColors(std::span<RenderColor const>(model.VertexColors()));
        GUARDR(renderModel->IsValid(), result)

        Add(result, *renderModel);
        return result;
    });

    PlanUIFunc planUIFunc = [this](auto args) {
        args.planner.PickerColor({ .label = "Color",
                                   .binding = { [this]() { return model.GetColor(); },
                                                [this](auto& value) { model.SetColor(value); } } });

        args.planner.Button({ .label = "Rebuild", .func = [this]() {
                                 model.SetMeshNeedsBuild();
                                 model.BuildIfNeeded();
                             } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

VectorList<RenderModel> MaterialRenderer::RenderModels() {
    auto result = model.RenderModels();

    // Always update the matrix for latest transform
    for (auto& renderModel : result) {
        renderModel.matrix = ModelMatrix();
    }

    return result;
}
