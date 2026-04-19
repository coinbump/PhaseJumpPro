#include "Renderer.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModel.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace PJ;

MaterialRendererCore::MaterialRendererCore(SomeWorldComponent* owner, Vector3 worldSize) :
    owner(owner),
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
}

WorldComponent::PlanUIFunc MaterialRendererCore::MakePlanUIFunc() {
    return [this](auto args) {
        args.planner.PickerColor(
            { .label = "Color",
              .binding =
                  Binding<Color>({ .getFunc = [this]() { return model.GetColor(); },
                                   .setFunc = [this](auto& value) { model.SetColor(value); } }) }
        );

        args.planner.Button({ .label = "Rebuild", .func = [this]() {
                                 model.SetMeshNeedsBuild();
                                 model.BuildIfNeeded();
                             } });
    };
}

VectorList<RenderModel> MaterialRendererCore::RenderModels() {
    auto result = model.RenderModels();

    // Always update the matrix for latest transform
    if (owner) {
        auto matrix = owner->ModelMatrix();
        for (auto& renderModel : result) {
            renderModel.matrix = matrix;
        }
    }

    return result;
}
