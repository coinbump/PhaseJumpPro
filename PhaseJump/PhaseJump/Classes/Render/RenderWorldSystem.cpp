#include "RenderWorldSystem.h"
#include "Camera.h"
#include "MaterialRenderModel.h"
#include "RenderContextModel.h"
#include "Renderer.h"
#include "RenderFeature.h"
#include "RenderPassBuilder.h"
#include "RenderProcessingModel.h"
#include "RenderProcessor.h"
#include "SomeRenderContext.h"
#include "SomeRenderEngine.h"
#include "StencilPopRenderModel.h"
#include "StencilPushRenderModel.h"
#include "UnorderedSet.h"
#include "ViewportRenderModel.h"
#include "World.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

namespace {
    /// @return True if `node` is a strict descendant of `ancestor` (not equal to it).
    /// Hardened against runaway parent chains — a dangling or cyclic parent pointer in
    /// a nested-scene setup (e.g. DesktopAppScene hosting PaintAppScene in a window) would
    /// otherwise walk forever and crash the camera sort.
    bool IsDescendant(WorldNode const* ancestor, WorldNode const* node) {
        GUARDR(ancestor && node, false)
        GUARDR(node != ancestor, false)

        // TODO: re-evaluate the need for this (test opening app in desktop)
        constexpr int maxDepth = 10000;
        int depth = 0;
        for (auto cursor = node->Parent(); cursor; cursor = cursor->Parent()) {
            if (cursor == ancestor) {
                return true;
            }
            if (++depth > maxDepth) {
                return false;
            }
        }
        return false;
    }

    /// Capture the fields of interest from a render model into a RenderInfo
    void FillRenderInfoItems(SomeRenderModel const& model, RenderInfo& info) {
        info.items.push_back({ .label = "Render type", .value = model.type });

        if (auto* material = dynamic_cast<MaterialRenderModel const*>(&model)) {
            bool const isBlendEnabled = material->IsFeatureEnabled(RenderFeature::Blend);
            info.items.push_back({ .label = "Blend feature",
                                   .value = isBlendEnabled ? "Enabled" : "Disabled" });
        }
    }

    /// Reduce a render-order DAG subtree into a tree of RenderInfo, mirroring the
    /// structure without copying the underlying render models.
    UP<RenderInfo> BuildRenderInfo(RenderPassModel::Node const& dagNode, bool isRoot) {
        auto info = NEW<RenderInfo>();
        if (dagNode.core) {
            FillRenderInfoItems(*dagNode.core, *info);
        } else if (isRoot) {
            info->items.push_back({ .label = "Render type", .value = "(pass root)" });
        }

        for (auto& edge : dagNode.Edges()) {
            GUARD_CONTINUE(edge && edge->toNode && edge->toNode->Value())
            auto child = edge->toNode->Value();
            auto childNode = SCAST<RenderPassModel::Node>(child);
            GUARD_CONTINUE(childNode)
            info->tree.Add(BuildRenderInfo(*childNode, false));
        }
        return info;
    }

    /// Count MaterialRenderModel nodes in the subtree under node
    /// Batching is handled by the batch render processor
    int CountMaterialNodes(RenderPassModel::NodeSharedPtr const& node) {
        GUARDR(node, 0)
        int count = 0;
        if (dynamic_cast<MaterialRenderModel const*>(node->core.get())) {
            ++count;
        }
        for (auto& edge : node->Edges()) {
            GUARD_CONTINUE(edge && edge->toNode)

            auto child = edge->toNode->Value();
            GUARD_CONTINUE(child)

            count += CountMaterialNodes(SCAST<RenderPassModel::Node>(child));
        }
        return count;
    }
} // namespace

RenderWorldSystem::RenderWorldSystem(String name) :
    Base(name) {
    model.phaseModel.onPhaseChangeFunc = [this](auto& phaseModel, auto phase) {
        // System-wide processors scope to the whole pass: rootNode is the pass root, and
        // its children are the per-camera DAG nodes. Set on the model when the pass DAG
        // is assembled in Render().
        model.processingModel.ProcessPhase(phase, systemPhaseRootNode);
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
    auto viewportEnd =
        std::stable_partition(cameras.begin(), cameras.end(), [](Camera const* camera) {
            return camera && camera->type == CameraType::Viewport;
        });

    // Nested viewports must render innermost-first so an enclosing viewport can sample the
    // nested viewport's freshly-written buffer when it draws the composite sprite. A deeper
    // owner sorts earlier. We key on tree depth rather than pairwise ancestry so the
    // comparator is a guaranteed strict weak ordering — a malformed (cyclic or dangling)
    // parent chain in a nested-window scene would otherwise make IsDescendant return true
    // in both directions and trip libc++'s hardening check inside std::stable_sort.
    auto depthOf = [](WorldNode const* node) {
        int depth = 0;
        constexpr int maxDepth = 10000;
        for (auto cursor = node; cursor; cursor = cursor->Parent()) {
            if (++depth > maxDepth) {
                break;
            }
        }
        return depth;
    };
    std::stable_sort(cameras.begin(), viewportEnd, [&](Camera const* a, Camera const* b) {
        GUARDR(a && b && a->owner && b->owner, false)
        return depthOf(a->owner) > depthOf(b->owner);
    });

    // Collect the owner nodes of all Viewport cameras. Their descendants are rendered exclusively
    // by their own viewport camera and must be hidden from every other camera in this pass.
    VectorList<WorldNode const*> viewportCameraNodes;
    for (auto& camera : cameras) {
        if (camera && camera->type == CameraType::Viewport && camera->owner) {
            viewportCameraNodes.push_back(camera->owner);
        }
    }

    // Phase 1: build the render-order DAG — a single pass-wide graph rooted at a pass whose
    // direct children are camera nodes (RenderCameraModels). Each camera node parents its
    // own render subtree. Viewport cameras come first (innermost to outermost, already
    // ordered in `cameras`); non-viewport cameras follow.
    auto renderPass = MAKE<RenderPassModel>();
    _contextModel.renderPass = renderPass;
    systemPhaseRootNode = renderPass->rootNode.get();

    struct CameraSlot {
        Camera* camera{};
        bool isViewport{};
        RenderContext* context{};
        VectorList<WorldNode*> nodesList;
        VectorList<MaterialRenderModel> renderModelsList;
        SP<RenderCameraModel> cameraModel;
        RenderPassModel::NodeSharedPtr cameraDagNode;
    };

    // UPs so `nodesList`/`renderModelsList` addresses stay stable while the RenderCameraModel
    // holds references into them.
    VectorList<UP<CameraSlot>> slots;

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
            if (isViewportCamera) {
                if (!IsDescendant(viewportRoot, node)) {
                    return false;
                }
                for (auto* vpRoot : viewportCameraNodes) {
                    if (vpRoot == viewportRoot) {
                        continue;
                    }
                    if (IsDescendant(viewportRoot, vpRoot) && IsDescendant(vpRoot, node)) {
                        return false;
                    }
                }
                return true;
            }
            for (auto* vpRoot : viewportCameraNodes) {
                if (IsDescendant(vpRoot, node)) {
                    return false;
                }
            }
            return true;
        };

        auto slot = NEW<CameraSlot>();
        slot->camera = camera;
        slot->isViewport = isViewportCamera;
        SomeRenderContext* _context =
            camera->renderContext ? camera->renderContext.get() : mainContext;
        slot->context = dynamic_cast<RenderContext*>(_context);

        slot->nodesList.reserve(_contextModel.nodes.size());
        slot->renderModelsList.reserve(_contextModel.nodes.size());

        VectorList<RenderPassBuilder::Entry> dagEntries;

        for (auto& node : _contextModel.nodes) {
            GUARD_CONTINUE(node->IsVisible())
            GUARD_CONTINUE(shouldRender(node))

            VectorList<Renderer*> renderers;
            node->CollectTypeComponents<Renderer>(renderers);
            for (auto& renderer : renderers) {
                GUARD_CONTINUE(renderer->IsEnabled())

                renderer->Signal(SignalId::RenderPrepare, Event<>{});

                auto thisRenderModels = renderer->RenderModels();
                auto thisPostRenderModels = renderer->PostRenderModels();
                GUARD_CONTINUE(!thisRenderModels.empty() || !thisPostRenderModels.empty())

                if (!thisRenderModels.empty()) {
                    if (auto* firstMaterial =
                            dynamic_cast<MaterialRenderModel*>(thisRenderModels[0].get())) {
                        GUARD_CONTINUE(firstMaterial->IsValid())
                    }
                }

                for (auto& sp : thisRenderModels) {
                    auto* material = dynamic_cast<MaterialRenderModel*>(sp.get());
                    if (material) {
                        slot->renderModelsList.push_back(*material);
                    }
                }
                slot->nodesList.push_back(node);
                dagEntries.push_back(RenderPassBuilder::Entry{
                    .worldNode = node,
                    .preModels = std::move(thisRenderModels),
                    .postModels = std::move(thisPostRenderModels) });
            }
        }

        // Construct the camera model. References to slot->nodesList/renderModelsList stay
        // valid for the dispatch phase because `slots` owns `slot` via UP.
        slot->cameraModel = MAKE<RenderCameraModel>(RenderCameraModel::Config{
            .context = slot->context, .camera = *camera, .nodes = slot->nodesList });
        if (isViewportCamera) {
            // Viewport cameras carry the "viewport" type so blend-inheritance and other
            // DAG walks treat this as a boundary.
            slot->cameraModel->type = RenderModelType::Viewport;
        }

        slot->cameraDagNode = renderPass->Add(slot->cameraModel);

        // Build the camera's render subtree under its DAG node.
        RenderPassBuilder dagBuilder;
        dagBuilder.BuildInto(*renderPass, slot->cameraDagNode, dagEntries);

        slots.push_back(std::move(slot));
    }

    // Blend inheritance runs once over the whole pass-wide DAG. Camera/viewport boundaries
    // (both typed RenderCameraModel) stop inheritance so opaque children of one camera's
    // blended parent don't leak into another camera's subtree.
    RenderPassBuilder::ApplyBlendInheritance(*renderPass);

    // Phase 2: Render cameras in order
    for (auto& slot : slots) {
        auto* camera = slot->camera;
        auto* context = slot->context;
        auto& cameraModel = *slot->cameraModel;

        cameraModel.phaseModel.onPhaseChangeFunc = [&](auto& phaseModel, auto phase) {
            // Camera-scoped processors see just this camera's DAG node — their rootNode
            // subtree is the camera's render content.
            camera->processingModel.ProcessPhase(phase, slot->cameraDagNode.get());
        };

        auto SetPhase = [&](String phase) {
            model.phaseModel.SetPhase(phase);
            cameraModel.phaseModel.SetPhase(phase);
        };

        SetPhase(RenderPhaseId::BindPrepare);
        context->Bind();
        SetPhase(RenderPhaseId::BindPost);

        context->clearColor = camera->clearColor;

        SetPhase(RenderPhaseId::ClearPrepare);

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

        if (slot->cameraDagNode) {
            // Count post-batch material nodes — these are the actual GPU draws this camera
            // will issue. Reporting `renderModelsList.size()` (pre-batch) gave a stable-720
            // reading that masked batching regressions.
            drawCount += CountMaterialNodes(slot->cameraDagNode);
            renderEngine.RenderDrawSubtree(*slot->cameraDagNode);
        }

        SetPhase(RenderPhaseId::DrawPost);
    }

    model.phaseModel.SetPhase(RenderPhaseId::RenderPassPresentPrepare);
    mainContext->Present();
    model.phaseModel.SetPhase(RenderPhaseId::RenderPassPresentPost);

    // Capture the final pass structure for editor inspection. Mirrors the DAG as a tree
    // of RenderInfo so the editor can show the graph without holding onto heavy models
    if (renderPass && renderPass->rootNode) {
        lastRenderInfo = BuildRenderInfo(*renderPass->rootNode, true);
    }

    // Drop the borrowed pointer before the pass DAG falls out of scope at return. A stray
    // system phase emission between frames would otherwise dereference a dead node.
    systemPhaseRootNode = nullptr;

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
    nodes(config.nodes) {
    type = RenderModelType::Camera;
}

void RenderCameraModel::SetPhase(String value) {
    phaseModel.SetPhase(value);
}

SP<RenderMaterial> RenderCameraModel::OverrideMaterial(MaterialRenderModel const& model) {
    auto i = overrideMaterials.find(model.Material());
    GUARDR(i != overrideMaterials.end(), nullptr)

    return i->second;
}

SP<RenderMaterial> RenderCameraModel::MakeOverrideMaterial(MaterialRenderModel const& model) {
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
