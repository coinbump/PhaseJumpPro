#include "RenderWorldSystem.h"
#include "RenderContextModel.h"
#include "RenderProcessingModel.h"
#include "RenderProcessor.h"
#include "SomeCamera.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "SomeRenderer.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

std::optional<RenderResult> RenderWorldSystem::Render(RenderContextModel& _contextModel) {
    auto context = _contextModel.renderContext;
    GUARDR(context && world, std::nullopt);

    model.phaseModel.onPhaseChangeFunc = [&](auto& phaseModel, auto phase) {
        model.processingModel.ProcessPhase(phase);
    };

    context->renderEngine.ResetForRenderPass();

    auto mainCamera = world->MainCamera();

    model.phaseModel.SetPhase(RenderPhase::PrepareBind);
    context->Bind();
    model.phaseModel.SetPhase(RenderPhase::PostBind);

    if (mainCamera) {
        context->clearColor = mainCamera->clearColor;
    }
    context->Clear();

    context->renderEngine.SetIsContextCleared(context->renderId, true);
    model.phaseModel.SetPhase(RenderPhase::PostClear);

    int drawCount{};

    auto& renderEngine = context->renderEngine;

    VectorList<RenderModel> models;
    models.reserve(_contextModel.nodes.size());

    {
#ifdef PROFILE
        DevProfiler devProfiler("System- Make RenderModels", [](String value) {
            std::cout << value;
        });

        cout << "Profile: Nodes count: " << contextModel.nodes.size() << std::endl;
#endif

        // FUTURE: can this be done on parallel threads?
        for (auto& node : _contextModel.nodes) {
            // Skip hidden nodes
            GUARD_CONTINUE(node->IsVisible())

            VectorList<SomeRenderer*> renderers;
            node->Signal(SignalId::RenderPrepare, Event<>{});
            node->CollectTypeComponents<SomeRenderer>(renderers);
            for (auto& renderer : renderers) {
                GUARD_CONTINUE(renderer->IsEnabled())

                /// Most renderers produce render models, for a list of draw calls to be sent to the
                /// GPU by the graphics engine
                auto thisRenderModels = renderer->RenderModels();
                auto thisRenderModelCount = thisRenderModels.size();

                GUARD_CONTINUE(thisRenderModelCount > 0)

                // Don't add junk models, it can break the render
                GUARD_CONTINUE(thisRenderModels[0].IsValid())

                AddRange(models, thisRenderModels);
            }
        }
    }

    auto& cameras = _contextModel.cameras;

    // Execute a complete render pass for each camera
    // Example: Camera A renders moving content, camera B renders fixed UI
    // FUTURE: support limiting which nodes are rendered for each camera,
    // either by using frustrum culling or a filter
    // FUTURE: support occlusion filtering for nodes based on camera view
    for (auto& camera : cameras) {
        GUARD_CONTINUE(camera)

        RenderContextModel contextModel = _contextModel;

        RenderCameraModel cameraModel(contextModel, *camera, models);
        cameraModel.phaseModel.onPhaseChangeFunc = [&](auto& phaseModel, auto phase) {
            // TODO: prepend "camera." for phase here?
            // Render pipeline shared by all cameras
            model.processingModel.ProcessPhase(cameraModel, phase);

            // Each camera can have its own render pipeline
            camera->processingModel.ProcessPhase(cameraModel, phase);
        };

        auto SetPhase = [&](String phase) {
            model.phaseModel.SetPhase(phase);
            cameraModel.phaseModel.SetPhase(phase);
        };

        SetPhase(RenderPhase::PrepareCamera);

        cameraModel.renderContext->Bind();

        if (!context->renderEngine.IsContextCleared(cameraModel.renderContext->renderId)) {
            cameraModel.renderContext->Clear();
            context->renderEngine.SetIsContextCleared(cameraModel.renderContext->renderId, true);
        }

        renderEngine.RenderStart(contextModel);
        camera->PreRender(contextModel);

        SetPhase(RenderPhase::Camera);

#ifdef PROFILE
        DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) { cout << value; });
#endif

        RenderDrawModel drawModel;
        drawModel.models = cameraModel.models;
        drawCount += (int)drawModel.models.size();

        SetPhase(RenderPhase::PrepareCameraDraw);
        renderEngine.RenderDraw(drawModel);
        SetPhase(RenderPhase::PostCameraDraw);

        SetPhase(RenderPhase::PostCamera);
    }

    model.phaseModel.SetPhase(RenderPhase::PreparePresent);
    context->Present();
    model.phaseModel.SetPhase(RenderPhase::PostPresent);

    RenderResult result;
    result.tags.Set(RenderStatId::DrawCount, drawCount);
    return result;
}

void RenderWorldSystem::Add(SP<RenderProcessor> processor) {
    model.processingModel.Add(processor);
    processor->system = this;
}

RenderWorldSystem::ProcessorList const& RenderWorldSystem::Processors() const {
    return model.processingModel.Processors();
}

// MARK: RenderCameraModel

RenderCameraModel::RenderCameraModel(
    RenderContextModel& contextModel, SomeCamera& camera, VectorList<RenderModel> models
) :
    renderContext(camera.renderContext ? camera.renderContext.get() : contextModel.renderContext),
    root(contextModel.root),
    nodes(contextModel.nodes),
    camera(&camera),
    models(models) {}

void RenderCameraModel::SetPhase(String value) {
    phaseModel.SetPhase(value);
}

SP<RenderMaterial> RenderCameraModel::OverrideMaterial(RenderModel const& model) {
    auto i = overrideMaterials.find(model.Material());
    GUARDR(i != overrideMaterials.end(), nullptr)

    return i->second;
}

SP<RenderMaterial> RenderCameraModel::MakeOverrideMaterial(RenderModel const& model) {
    auto baseMaterial = model.Material();
    GUARDR(baseMaterial, nullptr)

    auto i = overrideMaterials.find(baseMaterial);
    GUARDR(i == overrideMaterials.end(), i->second)

    auto result = MAKE<RenderMaterial>();
    *result = *baseMaterial;
    overrideMaterials.insert_or_assign(baseMaterial, result);

    return result;
}

// MARK: RenderPhaseModel

void RenderPhaseModel::SetPhase(String value) {
    GUARD(phase != value);
    phase = value;

    GUARD(onPhaseChangeFunc)
    onPhaseChangeFunc(*this, phase);
}
