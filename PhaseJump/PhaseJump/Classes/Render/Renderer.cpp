#include "Renderer.h"
#include "MaterialRenderModel.h"
#include "RenderContextModel.h"
#include "RenderMaterial.h"
#include "RenderModelBuilder.h"
#include "SomeRenderEngine.h"
#include "UIPlanner.h"

using namespace PJ;

MaterialRendererCore::MaterialRendererCore(SomeWorldComponent* owner, Vector3 worldSize) :
    owner(owner),
    model(worldSize) {

    model.SetBuildRenderModelsFunc([this](auto& model) {
        VectorList<Texture*> textures;
        VectorList<MaterialRenderModel> result;

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

RenderModelList MaterialRendererCore::RenderModels() {
    auto const& materialModels = model.RenderModels();

    Matrix4x4 matrix;
    bool const hasOwner = owner != nullptr;
    if (hasOwner) {
        matrix = owner->ModelMatrix();
    }

    // FUTURE: reduce large mesh copies here
    RenderModelList result;
    result.reserve(materialModels.size());
    for (auto const& materialModel : materialModels) {
        auto sp = MAKE<MaterialRenderModel>(materialModel);
        if (hasOwner) {
            sp->matrix = matrix;
        }
        result.push_back(sp);
    }
    return result;
}
