#pragma once

#include "GraphNode.h"
#include "StandardEdgeCore.h"
#include "StrongReference.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /**
     Acyclic graph node, for trees. Edges are directed from parent to child. Loops are not allowed

     This is a heavyweight object and in general you should probably use TreeNode/Treeable instead.
     The purpose of this class is to allow you to attach properties to graph edges
     */
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class AcyclicGraphNode : public GraphNode<EdgeCore, Core> {
    public:
        using This = AcyclicGraphNode<EdgeCore, Core>;
        using Base = GraphNode<EdgeCore, Core>;
        using AcyclicNode = This;
        using AcyclicNodeSharedPtr = SP<AcyclicNode>;
        using NodeStrongReference = StrongReference<GraphNode<EdgeCore>>;

        AcyclicNode* Root() {
            auto node = this;
            UnorderedSet<AcyclicNode*> searchedNodes;

            while (node->FromNodes().size() > 0) {
                // Prevent infinite loop for incorrect graphs
                if (Contains(searchedNodes, node)) {
                    return nullptr;
                }

                searchedNodes.insert(node);

                // A tree node can only have 1 fromNode (the parent)
                auto fromNode = *node->FromNodes().begin();
                if (fromNode) {
                    node = As<This>(fromNode);
                }
            }

            return As<This>(node);
        }

        AcyclicNode* Parent() const {
            GUARDR(!IsEmpty(this->FromNodes()), {});
            return As<This>(*this->FromNodes().begin());
        }

        bool IsRoot() const {
            return this->FromNodes().size() <= 0;
        }

        // MARK: GraphNode

        typename Base::NodeSharedPtr
        AddEdge(typename Base::NodeSharedPtr toNode, EdgeCore model = {}) override {
            // Acyclic graph nodes hold a strong reference to their next node
            this->AddEdgeInternal(model, NEW<NodeStrongReference>(toNode));
            return toNode;
        }
    };
} // namespace PJ
