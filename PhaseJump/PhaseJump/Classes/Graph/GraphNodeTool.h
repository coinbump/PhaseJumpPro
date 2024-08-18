#pragma once

#include "SomeGraphNode.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    template <class EdgeCore = StandardEdgeCore>
    struct GraphNodeTool {
        using Node = SomeGraphNode<EdgeCore>;
        using NodeSharedPtr = SP<Node>;
        using NodeSet = OrderedSet<NodeSharedPtr>;
        using NodeList = VectorList<NodeSharedPtr>;

        /// Collect all forward-directed nodes from this node
        NodeSet CollectGraph(SP<Node> fromNode) {
            NodeSet nodes;
            nodes.insert(fromNode);

            NodeSet searchedNodes;
            searchedNodes.insert(fromNode);

            return CollectEdgeNodesTo(fromNode, nodes, searchedNodes, true);
        }

        // TODO: should NodeList be a template parameter type?
        /// Collects depth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        NodeList CollectDepthFirstGraph(SP<Node> fromNode) {
            NodeList result;
            result.Add(fromNode);

            NodeSet searchedNodes;
            searchedNodes.insert(fromNode);

            CollectDepthFirstChildren(result, fromNode, searchedNodes);
            return result;
        }

        /// Collects breadth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        NodeList CollectBreadthFirstGraph(SP<Node> fromNode) {
            NodeList result;
            result.Add(fromNode);

            NodeSet searchedNodes;
            searchedNodes.insert(fromNode);

            NodeSet allNodes;
            allNodes.insert(fromNode);

            CollectBreadthFirstChildren(result, fromNode, searchedNodes, allNodes);
            return result;
        }

        NodeSet CollectConnectedTo(SP<Node> fromNode, bool isDeep) {
            NodeSet nodes;
            NodeSet searchedNodes;
            return CollectEdgeNodesTo(fromNode, nodes, searchedNodes, isDeep);
        }

    protected:
        NodeSet
        CollectEdgeNodesTo(SP<Node> fromNode, NodeSet& nodes, NodeSet& searchedNodes, bool isDeep) {
            auto iterEdges = fromNode->Edges();
            for (auto& edge : iterEdges) {
                auto toNode = edge->toNode;
                nodes.insert(toNode->Value());

                // Prevent infinite loop
                if (isDeep && !Contains(searchedNodes, toNode->Value())) {
                    searchedNodes.insert(toNode->Value());
                    CollectEdgeNodesTo(toNode->Value(), nodes, searchedNodes, true);
                }
            }

            return nodes;
        }

        void CollectBreadthFirstChildren(
            NodeList& nodes, NodeSharedPtr fromNode, NodeSet& searchedNodes, NodeSet& allNodes
        ) {
            for (auto& edge : fromNode->Edges()) {
                auto toNode = edge->toNode;

                // Avoid duplicates for graphs with loops
                if (Contains(allNodes, toNode->Value())) {
                    continue;
                }

                allNodes.insert(toNode->Value());
                nodes.Add(toNode->Value());
            }

            for (auto& edge : fromNode->Edges()) {
                auto toNode = edge->toNode;

                // Avoid infinite loop for graphs with loops
                if (Contains(searchedNodes, toNode->Value())) {
                    continue;
                }

                searchedNodes.insert(toNode->Value());
                CollectBreadthFirstChildren(nodes, toNode->Value(), searchedNodes, allNodes);
            }
        }

        void
        CollectDepthFirstChildren(NodeList& nodes, NodeSharedPtr fromNode, NodeSet& searchedNodes) {
            if (nullptr == fromNode) {
                return;
            }

            for (auto& edge : fromNode->Edges()) {
                auto toNode = edge->toNode;

                // Avoid infinite loop for graphs with loops
                if (Contains(searchedNodes, toNode->Value())) {
                    continue;
                }

                searchedNodes.insert(toNode->Value());
                nodes.Add(toNode->Value());
                CollectDepthFirstChildren(nodes, toNode->Value(), searchedNodes);
            }
        }
    };
} // namespace PJ
