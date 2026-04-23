#pragma once

#include "AcyclicGraphNode.h"
#include "SomeRenderModel.h"
#include "StandardEdgeCore.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 4/22/26
 */
namespace PJ {
    /// Container for a single render pass DAG of render models.
    class RenderPassModel {
    public:
        using This = RenderPassModel;
        using ModelSharedPtr = SP<SomeRenderModel>;
        using Node = AcyclicGraphNode<StandardEdgeCore, ModelSharedPtr>;
        using NodeSharedPtr = SP<Node>;

        /// DAG root, has no render model
        NodeSharedPtr rootNode;

        RenderPassModel() {
            rootNode = MAKE<Node>();
        }

        /// Adds model as child of parent
        NodeSharedPtr Add(ModelSharedPtr model, NodeSharedPtr parent = {}) {
            auto node = MAKE<Node>();
            node->core = model;
            if (model) {
                model->node = node.get();
            }

            auto& _parent = parent ? parent : rootNode;
            _parent->AddEdge(node);
            return node;
        }
    };
} // namespace PJ
