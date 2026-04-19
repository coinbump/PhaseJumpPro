#include "RenderWorldSystem.h"
#include "Camera.h"
#include "RenderContextModel.h"
#include "Renderer.h"
#include "RenderProcessingModel.h"
#include "RenderProcessor.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "UnorderedSet.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

namespace {
    /// @return True if `node` is a strict descendant of `ancestor` (not equal to it).
    bool IsDescendant(WorldNode const* ancestor, WorldNode const* node) {
        GUARDR(ancestor && node, false)
        GUARDR(node != ancestor, false)

        for (auto cursor = node->Parent(); cursor; cursor = cursor->Parent()) {
            if (cursor == ancestor) {
                return true;
            }
        }
        return false;
    }
} // namespace

RenderWorldSystem::RenderWorldSystem(String name) :
    Base(name) {
    model.phaseModel.onPhaseChangeFunc = [this](auto& phaseModel, auto phase) {
        // Shared render pipeline for all cameras
        model.processingModel.ProcessPhase(phase, {});
    };
}

std::optional<RenderResult> RenderWorldSystem::Render(RenderContextModel& _contextModel) {
    auto mainContext = dynamic_cast<RenderContext*>(_contextModel.renderContext);
    GUARDR(mainContext && world, {});

    model.phaseModel.SetPhase(RenderPhaseId::RenderPassStartPrepare);

    auto& renderEngine = mainContext->renderEngine;
    renderEngine.RenderPassStart();

    model.phaseModel.SetPhase(RenderPhaseId::RenderPassStartPost);

    int drawCount{};

    auto& cameras = _contextModel.cameras;

    // Viewport cameras render first so their backing textures are up to date before any outer
    // camera samples them (e.g. a SpriteRenderer on the viewport owner composites the texture).
    std::stable_partition(cameras.begin(), cameras.end(), [](Camera const* camera) {
        return camera && camera->type == CameraType::Viewport;
    });

    // Collect the owner nodes of all Viewport cameras. Their descendants are rendered exclusively
    // by their own viewport camera and must be hidden from every other camera in this pass.
    VectorList<WorldNode const*> viewportCameraNodes;
    for (auto& camera : cameras) {
        if (camera && camera->type == CameraType::Viewport && camera->owner) {
            viewportCameraNodes.push_back(camera->owner);
        }
    }

    // Execute a complete render pass for each camera
    // FUTURE: support occlusion filtering for nodes based on camera view
    for (auto& camera : cameras) {
        GUARD_CONTINUE(camera)

        bool const isViewportCamera = camera->type == CameraType::Viewport;
        WorldNode const* viewportRoot = isViewportCamera ? camera->owner : nullptr;

        // Discard non-viewport cameras nested inside a viewport's subtree. Their content is
        // already rendered by the enclosing viewport camera into the viewport's own context;
        // letting them run again would double-render to the main context.
        if (!isViewportCamera) {
            bool nestedInViewport = false;
            for (auto* vpRoot : viewportCameraNodes) {
                if (IsDescendant(vpRoot, camera->owner)) {
                    nestedInViewport = true;
                    break;
                }
            }
            GUARD_CONTINUE(!nestedInViewport)
        }

        auto shouldRender = [&](WorldNode const* node) {
            // Viewport camera: render only the children below its owner (the owner itself lives in
            // the outer world and holds the sprite that composites this viewport back in).
            if (isViewportCamera) {
                return IsDescendant(viewportRoot, node);
            }

            // Non-viewport camera: render everything except nodes that are strict descendants of a
            // Viewport camera's owner. The viewport owner itself still renders via the main camera
            // (that's where the composite SpriteRenderer lives).
            for (auto* vpRoot : viewportCameraNodes) {
                if (IsDescendant(vpRoot, node)) {
                    return false;
                }
            }
            return true;
        };

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

                // Viewport-camera visibility gating (see shouldRender above).
                GUARD_CONTINUE(shouldRender(node))

                VectorList<Renderer*> renderers;
                node->CollectTypeComponents<Renderer>(renderers);
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
        SomeRenderContext* _context =
            camera->renderContext ? camera->renderContext.get() : mainContext;
        auto context = dynamic_cast<RenderContext*>(_context);

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

        SetPhase(RenderPhaseId::BindPrepare);
        context->Bind();
        SetPhase(RenderPhaseId::BindPost);

        context->clearColor = camera->clearColor;

        // FUTURE: add programmable pipeline that sets phase to prepare and post for each action
        SetPhase(RenderPhaseId::ClearPrepare);

        // If multiple cameras share the same render context, make sure we only clear it for the
        // first camera
        if (!context->renderEngine.IsContextCleared(cameraModel.renderContext->RenderId())) {
            cameraModel.renderContext->Clear();
            context->renderEngine.SetIsContextCleared(cameraModel.renderContext->RenderId(), true);
        }

        SetPhase(RenderPhaseId::ClearPost);

        renderEngine.RenderStart(context);
        camera->RenderStart(context);

#ifdef PROFILE
        DevProfiler devProfilerRenderCamera("Render- Camera", [](String value) { cout << value; });
#endif

        SetPhase(RenderPhaseId::DrawPrepare);

        RenderDrawModel drawModel{ .renderModels = cameraModel.renderModels };
        drawCount += (int)drawModel.renderModels.size();

        renderEngine.RenderDraw(drawModel);

        SetPhase(RenderPhaseId::DrawPost);
    }

    model.phaseModel.SetPhase(RenderPhaseId::RenderPassPresentPrepare);
    mainContext->Present();
    model.phaseModel.SetPhase(RenderPhaseId::RenderPassPresentPost);

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

RenderCameraModel::RenderCameraModel(RenderCameraModel::Config const& config) :
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
