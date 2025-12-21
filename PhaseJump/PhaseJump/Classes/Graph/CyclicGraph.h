#pragma once

#include "CyclicGraphNode.h"
#include "StandardEdgeCore.h"
#include "UnorderedSet.h"
#include "Updatable.h"
#include "WeakReference.h"
#include <memory>
#include <vector>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Supports a graph with cycles (State machines, story sequences, etc.)
    /// The graph holds on to nodes, and the nodes have weak references to each
    /// other
    template <class Node>
    class CyclicGraph : public Base {
    public:
        using EdgeCore = Node::EdgeCoreType;
        using NodeSharedPtr = SP<Node>;
        using NodeSet = UnorderedSet<NodeSharedPtr>;
        using Edge = Node::Edge;

        /// Holds strong references to nodes added to graph
        NodeSet nodes;

    protected:
        Node* rootNode{};

    public:
        Node* Root() const {
            return rootNode;
        }

        void SetRoot(Node* node) {
            rootNode = node;
        }

        void Remove(Node& node) {
            auto iterFromNodes = node.FromNodes();
            for (auto& fromNode : iterFromNodes) {
                fromNode->RemoveEdgesTo(node);
            }

            VectorList<Edge*> iterEdges =
                Map<Edge*>(node.Edges(), [](auto& edge) { return edge.get(); });
            for (auto& edge : iterEdges) {
                node.RemoveEdge(*edge);
            }

            if (&node == Root()) {
                SetRoot({});
            }
            nodes.erase(SCAST<Node>(node.shared_from_this()));
        }

        void AddEdge(SP<Node> fromNode, EdgeCore model, SP<Node> toNode) {
            GUARD(fromNode && toNode)

            fromNode->AddEdgeInternal(
                model, NEW<WeakReference<GraphNode<EdgeCore, typename Node::CoreType>>>(toNode)
            );

            nodes.insert(fromNode);
            nodes.insert(toNode);
        }
    };
} // namespace PJ
