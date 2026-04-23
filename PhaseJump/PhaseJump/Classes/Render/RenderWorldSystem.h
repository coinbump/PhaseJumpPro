#pragma once

#include "MaterialRenderModel.h"
#include "RenderCameraModel.h"
#include "RenderContextModel.h"
#include "RenderProcessingModel.h"
#include "SomeWorldSystem.h"
#include "TreeNode.h"
#include "VectorList.h"

/*
 RATING: 4 stars
 Tested and works. Missing full support for multiple cameras, offscreen view ports, and perspective
 cameras CODE REVIEW: 12/22/24
 */
namespace PJ {
    class RenderContextModel;

    /// Captured snapshot of a render DAG node, used by the editor to inspect the most
    /// recent render pass without copying expensive models
    class RenderInfo {
    public:
        using This = RenderInfo;
        using Tree = TreeNode<This, UP<This>>;

        /// Labeled field captured from a render model
        struct Item {
            String label;
            String value;
        };

        /// Captured fields for this DAG node (e.g. Render type, Blend feature)
        VectorList<Item> items;

        /// Child RenderInfo nodes that mirror the DAG children
        Tree tree;

        RenderInfo() :
            tree(*this) {}

        // MARK: Treeable

        This* Parent() const {
            return tree.Parent();
        }

        void SetParent(This* value) {
            tree.SetParent(value);
        }
    };

    /// Render pipeline logic for the world. Every world needs a render system
    class RenderWorldSystem : public WorldSystem {
    public:
        class Model {
        public:
            RenderPhaseModel phaseModel;
            RenderProcessingModel processingModel;
        };

        using Base = WorldSystem;
        using ProcessorList = RenderProcessingModel::ProcessorList;

    protected:
        Model model;

        /// Pass root for this frame. Set at the start of `Render` so system-wide processor
        /// phases can resolve to the whole pass DAG; null outside a render call.
        SomeRenderModelNode* systemPhaseRootNode{};

        /// Captured tree mirroring the most recent render-order DAG. Populated at the end
        /// of each `Render` call so the editor can inspect the pass structure between frames
        UP<RenderInfo> lastRenderInfo;

    public:
        RenderWorldSystem(String name = "Render");

        virtual ~RenderWorldSystem() {}

        void Add(SP<RenderProcessor> processor);

        ProcessorList const& Processors() const;

        /// @return Returns the most recent captured render-info tree, or nullptr if no
        /// render has run yet
        RenderInfo const* LastRenderInfo() const {
            return lastRenderInfo.get();
        }

        // MARK: SomeWorldSystem

        std::optional<RenderResult> Render(RenderContextModel& contextModel) override;
    };
} // namespace PJ
