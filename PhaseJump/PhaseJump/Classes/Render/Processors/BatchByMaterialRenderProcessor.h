#pragma once

#include "RenderProcessor.h"
#include "Utils.h"

/*
 RATING: 5 stars
 Tested and works
 CODE REVIEW: 12/8/24
 */
namespace PJ {
    struct MaterialRenderModel;

    /// Batches render models by material to reduce the total # of draw calls and improve
    /// render performance.
    ///
    /// DAG-native: walks the scope root node's subtree and collapses runs of contiguous
    /// MaterialRenderModel siblings that share a material into a single combined node.
    /// Any non-material node (stencil push/pop, viewport, camera) breaks the batch, so a
    /// run split by a stencil push becomes two separate batched draws.
    class BatchByMaterialRenderProcessor : public RenderProcessor {
    public:
        using Base = RenderProcessor;

        BatchByMaterialRenderProcessor() :
            Base({ .name = "Batch by material", .phases = { RenderPhaseId::DrawPrepare } }) {}

        /// Combine a run of same-material models into one. Public for tests.
        std::optional<MaterialRenderModel> Combine(VectorList<MaterialRenderModel*>& renderModels);

        /// Walk `parent`'s children and replace runs of same-material siblings with single
        /// combined nodes. Recurses into non-material children and into the combined node's
        /// own subtree. Public for tests.
        void BatchChildren(SomeRenderModelNode& parent);

        // MARK: RenderProcessor

        void Process(SomeRenderModelNode& rootNode) override;
    };
} // namespace PJ
