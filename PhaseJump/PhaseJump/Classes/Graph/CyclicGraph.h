#pragma once

#include "CyclicGraphNode.h"
#include "StandardEdgeCore.h"
#include "UnorderedMap.h"
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
        using NodeMap = UnorderedMap<Node*, NodeSharedPtr>;
        using Edge = Node::Edge;

        /// Strong references to every node in the graph, keyed by raw pointer so
        /// `Remove` and `AddNode` are O(1).
        NodeMap nodes;

    protected:
        Node* rootNode{};

    public:
        Node* Root() const {
            return rootNode;
        }

        void SetRoot(Node* node) {
            rootNode = node;
        }

        /// Insert a node into the graph.
        void AddNode(NodeSharedPtr const& node) {
            GUARD(node)
            nodes[node.get()] = node;
        }

        void Remove(Node& node) {
            auto iterFromNodes = node.FromNodes();
            for (auto& fromNode : iterFromNodes) {
                fromNode->RemoveEdgesTo(node);
            }

            node.RemoveAllEdges();

            if (&node == Root()) {
                SetRoot({});
            }

            nodes.erase(&node);
        }

        /// Drop every node and edge in one pass. Faster than removing individually
        void Clear() {
            // Clear outgoing edges first so any external reference that outlives the graph
            // doesn't observe stale `fromNodes` back-pointers to already-destroyed nodes.
            for (auto& entry : nodes) {
                if (entry.second) {
                    entry.second->RemoveAllEdges();
                }
            }
            SetRoot({});
            nodes.clear();
        }

        void AddEdge(SP<Node> fromNode, EdgeCore model, SP<Node> toNode) {
            GUARD(fromNode && toNode)

            fromNode->AddEdgeInternal(
                model, NEW<WeakReference<GraphNode<EdgeCore, typename Node::CoreType>>>(toNode)
            );

            AddNode(fromNode);
            AddNode(toNode);
        }
    };
} // namespace PJ
