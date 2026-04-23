#include "RenderPassBuilder.h"
#include "RenderFeature.h"
#include "StencilPushRenderModel.h"
#include "ViewportRenderModel.h"
#include "WorldNode.h"
#include <optional>

using namespace PJ;

namespace {
    using NodeSharedPtr = RenderPassModel::NodeSharedPtr;

    /// Recursive DFS that pushes a blend override down the DAG, stopping at viewport
    /// boundaries.
    void
    ApplyBlendDFS(NodeSharedPtr const& node, std::optional<RenderFeatureState> const& override) {
        GUARD(node)
        auto const& model = node->core;

        std::optional<RenderFeatureState> nextOverride = override;

        if (model) {
            // Viewport boundary — drop any inherited override before recursing.
            if (dynamic_cast<ViewportRenderModel const*>(model.get())) {
                nextOverride.reset();
            } else if (auto* material = dynamic_cast<MaterialRenderModel*>(model.get())) {
                // Inherit blend from a blended ancestor onto an opaque descendant.
                if (override.has_value()) {
                    material->overrideFeatures[RenderFeature::Blend] = *override;
                }
                // If this material is itself blended, descendants also become blended.
                if (material->IsFeatureEnabled(RenderFeature::Blend)) {
                    nextOverride = RenderFeatureState::Enable;
                }
            }
        }

        for (auto& edge : node->Edges()) {
            GUARD_CONTINUE(edge && edge->toNode)
            auto child = edge->toNode->Value();
            GUARD_CONTINUE(child)
            ApplyBlendDFS(SCAST<RenderPassModel::Node>(child), nextOverride);
        }
    }
} // namespace

SP<RenderPassModel>
RenderPassBuilder::Build(VectorList<Entry> const& entries, Config const& config) {
    auto pass = MAKE<RenderPassModel>();

    // If the caller supplied a wrapper (typically a ViewportRenderModel for viewport
    // cameras), insert it as a single child of the root and treat it as the effective root
    // for entry attachment. The DAG then literally encodes "everything below renders inside
    // this viewport" without a separate side-channel.
    NodeSharedPtr effectiveRoot = pass->rootNode;
    if (config.wrapperModel) {
        effectiveRoot = pass->Add(config.wrapperModel);
    }

    BuildInto(*pass, effectiveRoot, entries);
    return pass;
}

void RenderPassBuilder::BuildInto(
    RenderPassModel& pass, NodeSharedPtr anchor, VectorList<Entry> const& entries
) {
    UnorderedMap<WorldNode const*, NodeSharedPtr> deepestPreFor;
    VectorList<PerEntry> perEntry;
    perEntry.reserve(entries.size());

    // Pass 1: pre models in forward (tree) order.
    for (auto const& entry : entries) {
        PerEntry slot;
        slot.postModels = entry.postModels;

        if (entry.preModels.empty()) {
            perEntry.push_back(std::move(slot));
            continue;
        }

        auto parent = DagParentFor(entry.worldNode, deepestPreFor, anchor);
        slot.dagParentForPost = parent;

        auto deepest = AddChain(pass, parent, entry.preModels);
        if (entry.worldNode) {
            deepestPreFor[entry.worldNode] = deepest;
        }
        perEntry.push_back(std::move(slot));
    }

    // Pass 2: post models in reverse order so a parent's post lands after its descendants
    // in DFS pre-order.
    for (size_t i = entries.size(); i > 0; --i) {
        auto& slot = perEntry[i - 1];
        if (slot.postModels.empty() || !slot.dagParentForPost) {
            continue;
        }
        AddChain(pass, slot.dagParentForPost, slot.postModels);
    }
}

RenderPassBuilder::NodeSharedPtr RenderPassBuilder::DagParentFor(
    WorldNode const* worldNode, UnorderedMap<WorldNode const*, NodeSharedPtr> const& deepestPreFor,
    NodeSharedPtr root
) {
    GUARDR(worldNode, root)
    for (auto cursor = worldNode->Parent(); cursor; cursor = cursor->Parent()) {
        auto it = deepestPreFor.find(cursor);
        if (it != deepestPreFor.end()) {
            return it->second;
        }
    }
    return root;
}

RenderPassBuilder::NodeSharedPtr RenderPassBuilder::AddChain(
    RenderPassModel& pass, NodeSharedPtr parent, RenderModelList const& models
) {
    // Wrapper models (stencil push, viewport) nest — each becomes the parent for the
    // models that follow, so descendants attach inside the deepest wrapper. Non-wrapper
    // models (plain MaterialRenderModel draws) are siblings at the current scope — a
    // renderer emitting N draws (ImRenderer's common case) turns into N siblings under
    // the same parent, which is what BatchByMaterial needs to collapse them.
    //
    // Return value: the "deepest" DAG node for this chain, used as the attach point for
    // descendant world nodes. Wrappers advance this; a plain sibling also counts as the
    // deepest when no wrapper is open, so a child world node's materials hang under its
    // parent's material (required for blend inheritance through the DAG).
    auto cursor = parent;
    NodeSharedPtr deepest = parent;
    for (auto const& model : models) {
        bool const isScopeWrapper = dynamic_cast<StencilPushRenderModel const*>(model.get()) ||
                                    dynamic_cast<ViewportRenderModel const*>(model.get());
        if (isScopeWrapper) {
            cursor = pass.Add(model, cursor);
            deepest = cursor;
        } else {
            deepest = pass.Add(model, cursor);
        }
    }
    return deepest;
}

void RenderPassBuilder::ApplyBlendInheritance(RenderPassModel& pass) {
    GUARD(pass.rootNode)
    ApplyBlendDFS(pass.rootNode, std::nullopt);
}
