#pragma once

#include "GraphNode.h"
#include <queue>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 1/9/25
 */
namespace PJ {
    template <class EdgeCore = StandardEdgeCore>
    struct GraphNodeTool {
        using Node = GraphNode<EdgeCore>;
        using NodeSharedPtr = SP<Node>;
        using NodeSet = OrderedSet<NodeSharedPtr>;
        using NodeList = VectorList<NodeSharedPtr>;
        using Edge = Node::Edge;

        /// Collect all forward-directed nodes from this node
        NodeSet CollectGraph(SP<Node> const& fromNode) {
            NodeSet nodes;
            nodes.insert(fromNode);

            NodeSet searchedNodes;
            searchedNodes.insert(fromNode);

            return CollectEdgeNodesTo(fromNode, nodes, searchedNodes, true);
        }

        /// Collects depth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        NodeList CollectDepthFirstGraph(SP<Node> const& fromNode) {
            NodeList result;
            Add(result, fromNode);

            NodeSet searchedNodes;
            searchedNodes.insert(fromNode);

            CollectDepthFirstChildren(result, fromNode, searchedNodes);
            return result;
        }

        /// Collects breadth-first ordered graph. Cycles are handled by `visited`.
        NodeList CollectBreadthFirstGraph(SP<Node> const& fromNode) {
            NodeList result;
            GUARDR(fromNode, result)

            NodeSet visited;
            std::queue<NodeSharedPtr> frontier;

            Add(result, fromNode);
            visited.insert(fromNode);
            frontier.push(fromNode);

            while (!frontier.empty()) {
                auto current = std::move(frontier.front());
                frontier.pop();

                for (auto& edge : current->Edges()) {
                    auto& toNode = edge->toNode;
                    GUARD_CONTINUE(toNode)

                    auto child = toNode->Value();
                    GUARD_CONTINUE(child)
                    if (Contains(visited, child)) {
                        continue;
                    }
                    visited.insert(child);
                    Add(result, child);
                    frontier.push(std::move(child));
                }
            }

            return result;
        }

        NodeSet CollectConnectedTo(SP<Node> const& fromNode, bool isDeep) {
            NodeSet nodes;
            NodeSet searchedNodes;
            return CollectEdgeNodesTo(fromNode, nodes, searchedNodes, isDeep);
        }

    protected:
        NodeSet CollectEdgeNodesTo(
            SP<Node> const& fromNode, NodeSet& nodes, NodeSet& searchedNodes, bool isDeep
        ) {
            VectorList<Edge*> iterEdges =
                Map<Edge*>(fromNode->Edges(), [](auto& edge) { return edge.get(); });
            for (auto& edge : iterEdges) {
                // Cache Value() — every call bumps an atomic refcount (and locks, for weak refs).
                auto child = edge->toNode->Value();
                GUARD_CONTINUE(child)

                nodes.insert(child);

                if (isDeep && !Contains(searchedNodes, child)) {
                    searchedNodes.insert(child);
                    CollectEdgeNodesTo(child, nodes, searchedNodes, true);
                }
            }

            return nodes;
        }

        void CollectDepthFirstChildren(
            NodeList& nodes, SP<Node> const& fromNode, NodeSet& searchedNodes
        ) {
            GUARD(fromNode)

            for (auto& edge : fromNode->Edges()) {
                auto child = edge->toNode->Value();
                GUARD_CONTINUE(child)

                if (Contains(searchedNodes, child)) {
                    continue;
                }

                searchedNodes.insert(child);
                Add(nodes, child);
                CollectDepthFirstChildren(nodes, child, searchedNodes);
            }
        }
    };
} // namespace PJ
