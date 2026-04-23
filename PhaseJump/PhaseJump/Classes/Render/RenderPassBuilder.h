#pragma once

#include "Renderer.h"
#include "RenderPassModel.h"
#include "UnorderedMap.h"
#include "VectorList.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    class WorldNode;

    /// Builds the per-camera DAG of SomeRenderModels from a list of renderers and their
    /// owning world nodes. Designed to be unit-testable in isolation from the full render
    /// pipeline.
    ///
    /// Algorithm:
    ///  - Pass 1 (tree order): for each renderer, attach its render models
    ///  - Pass 2 (reverse order): for each renderer that emitted  post render models, add them
    ///  after
    ///    the node's children
    class RenderPassBuilder {
    public:
        using This = RenderPassBuilder;
        using NodeSharedPtr = RenderPassModel::NodeSharedPtr;

        /// One renderer's contribution to the DAG, in world-tree order.
        struct Entry {
            WorldNode* worldNode{};
            RenderModelList preModels;
            RenderModelList postModels;
        };

        struct Config {
            /// Optional wrapper inserted directly under the root before any entries are
            /// processed. All entries attach under this wrapper instead of root. Use this
            /// for viewport cameras to mark their DAG with a ViewportRenderModel — the
            /// render engine then knows the entire subtree renders inside that viewport.
            SP<SomeRenderModel> wrapperModel;
        };

        /// Builds and returns the DAG. Entries must be supplied in world-tree order
        /// (parent before children)
        SP<RenderPassModel> Build(VectorList<Entry> const& entries, Config const& config = {});

        /// Used to assemble the render-order DAG where one pass-wide root holds per-camera subtrees
        void
        BuildInto(RenderPassModel& pass, NodeSharedPtr anchor, VectorList<Entry> const& entries);

        /// Walks the DAG and propagates Blend feature state from each blended node
        /// Stops at Viewport boundaries
        static void ApplyBlendInheritance(RenderPassModel& pass);

    protected:
        struct PerEntry {
            /// DAG parent that pre[0] was added under, used for post models
            NodeSharedPtr dagParentForPost;

            /// Renderer's PostRenderModels()
            RenderModelList postModels;
        };

        /// Walk up `worldNode`'s parent chain until we find one with a recorded DAG node.
        /// Returns the root node if no ancestor is found.
        NodeSharedPtr DagParentFor(
            WorldNode const* worldNode,
            UnorderedMap<WorldNode const*, NodeSharedPtr> const& deepestPreFor, NodeSharedPtr root
        );

        /// Adds `models` as a parent-child chain under `parent`. Returns the deepest node
        /// in the chain (or `parent` if `models` is empty).
        NodeSharedPtr
        AddChain(RenderPassModel& pass, NodeSharedPtr parent, RenderModelList const& models);
    };
} // namespace PJ
