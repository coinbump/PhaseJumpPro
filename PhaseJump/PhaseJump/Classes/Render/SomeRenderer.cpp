#include "SomeRenderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace PJ;

SomeMaterialRenderer::SomeMaterialRenderer(Vector3 worldSize) :
    model(worldSize) {

    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<SomeTexture*> textures;
        VectorList<RenderModel> result;

        RenderModelBuilder builder;
        auto renderModel = builder.Build(*this, model);
        GUARDR(renderModel, result)
        renderModel->SetVertexColors(std::span<RenderColor const>(model.VertexColors()));
        GUARDR(renderModel->IsValid(), result)

        Add(result, *renderModel);
        return result;
    });

    PlanUIFunc planUIFunc = [](auto& component, String context, UIPlanner& planner) {
        auto renderer = static_cast<SomeMaterialRenderer*>(&component);
        auto& model = renderer->model;

        planner.PickerColor({ .label = "Color",
                              .binding = { [&]() { return model.GetColor(); },
                                           [&](auto& value) { model.SetColor(value); } } });
    };
    Override(planUIFuncs[UIContextId::Inspector], planUIFunc);
}

VectorList<RenderModel> SomeMaterialRenderer::RenderModels() {
    auto result = model.RenderModels();

    // Always update the matrix for latest transform
    for (auto& renderModel : result) {
        renderModel.matrix = ModelMatrix();
    }

    return result;
}
