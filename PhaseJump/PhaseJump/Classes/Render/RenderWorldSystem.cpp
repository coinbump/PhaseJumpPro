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

RenderWorldSystem::RenderWorldSystem(String name) :
    Base(name) {
    model.phaseModel.onPhaseChangeFunc = [this](auto& phaseModel, auto phase) {
        // Shared render pipeline for all cameras
        model.processingModel.ProcessPhase(phase, {});
    };
}

std::optional<RenderResult> RenderWorldSystem::Render(RenderContextModel& _contextModel) {
    auto mainContext = _contextModel.renderContext;
    GUARDR(mainContext && world, {});

    model.phaseModel.SetPhase(RenderPhase::RenderPassStartPrepare);

    auto& renderEngine = mainContext->renderEngine;
    renderEngine.RenderPassStart();

    model.phaseModel.SetPhase(RenderPhase::RenderPassStartPost);

    int drawCount{};

    auto& cameras = _contextModel.cameras;

    // Execute a complete render pass for each camera
    // FUTURE: support limiting which nodes are rendered for each camera
    // FUTURE: support occlusion filtering for nodes based on camera view
    for (auto& camera : cameras) {
        GUARD_CONTINUE(camera)

        // Rendered nodes are sent to render processors to add custom render models
        // Example: show colliders render processor
        VectorList<WorldNode*> nodes;
        nodes.reserve(_contextModel.nodes.size());

        // Render models are sent to render engine
        VectorList<RenderModel> renderModels;
        renderModels.reserve(_contextModel.nodes.size());

        {
#ifdef PROFILE
            DevProfiler devProfiler("System- Make RenderModels", [](String value) {
                std::cout << value;
            });

            PJ::Log("Profile: Nodes count: ", contextModel.nodes.size());
#endif

            // FUTURE: can this be done on parallel threads?
            for (auto& node : _contextModel.nodes) {
                // Skip hidden nodes
                GUARD_CONTINUE(node->IsVisible())

                VectorList<SomeRenderer*> renderers;
                node->CollectTypeComponents<SomeRenderer>(renderers);
                for (auto& renderer : renderers) {
                    GUARD_CONTINUE(renderer->IsEnabled())

                    // Give renderer a chance to prepare. Used for immediate renderers
                    renderer->Signal(SignalId::RenderPrepare, Event<>{});

                    // A renderer produces render models to be sent to the render engine
                    auto thisRenderModels = renderer->RenderModels();
                    auto thisRenderModelCount = thisRenderModels.size();
                    GUARD_CONTINUE(thisRenderModelCount > 0)

                    // Don't add junk models, it can break the render
                    GUARD_CONTINUE(thisRenderModels[0].IsValid())

                    AddRange(renderModels, thisRenderModels);
                    nodes.push_back(node);
                }
            }
        }

        // Offscreen (viewport) cameras define their own render context
        SomeRenderContext* context =
            camera->renderContext ? camera->renderContext.get() : mainContext;

        RenderCameraModel cameraModel(
            { .camera = *camera, .renderModels = renderModels, .context = context, .nodes = nodes }
        );
        cameraModel.phaseModel.onPhaseChangeFunc = [&](auto& phaseModel, auto phase) {
            // Each camera can have its own render pipeline
            camera->processingModel.ProcessPhase(phase, &cameraModel);
        };

        auto SetPhase = [&](String phase) {
            model.phaseModel.SetPhase(phase);
            cameraModel.phaseModel.SetPhase(phase);
        };

        SetPhase(RenderPhase::BindPrepare);
        context->Bind();
        SetPhase(RenderPhase::BindPost);

        context->clearColor = camera->clearColor;

        // FUTURE: add programmable pipeline that sets phase to prepare and post for each action
        SetPhase(RenderPhase::ClearPrepare);

        // If multiple cameras share the same render context, make sure we only clear it for the
        // first camera
        if (!context->renderEngine.IsContextCleared(cameraModel.renderContext->renderId)) {
            cameraModel.renderContext->Clear();
            context->renderEngine.SetIsContextCleared(cameraModel.renderContext->renderId, true);
        }

        SetPhase(RenderPhase::ClearPost);

        renderEngine.RenderStart(context);
        camera->RenderStart(context);

#ifdef PROFILE
        DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) { cout << value; });
#endif

        SetPhase(RenderPhase::DrawPrepare);

        RenderDrawModel drawModel{ .models = cameraModel.renderModels };
        drawCount += (int)drawModel.models.size();

        renderEngine.RenderDraw(drawModel);

        SetPhase(RenderPhase::DrawPost);
    }

    model.phaseModel.SetPhase(RenderPhase::RenderPassPresentPrepare);
    mainContext->Present();
    model.phaseModel.SetPhase(RenderPhase::RenderPassPresentPost);

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

RenderCameraModel::RenderCameraModel(RenderCameraModel::Config config) :
    renderContext(config.camera.renderContext ? config.camera.renderContext.get() : config.context),
    camera(&config.camera),
    nodes(config.nodes),
    renderModels(config.renderModels) {}

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
