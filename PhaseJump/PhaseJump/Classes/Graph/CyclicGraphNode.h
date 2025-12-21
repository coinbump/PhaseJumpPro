#pragma once

#include "GraphNode.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Cyclic nodes are managed by CyclicGraph
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class CyclicGraphNode : public GraphNode<EdgeCore, Core> {
    public:
        using Base = GraphNode<EdgeCore, Core>;
        using EdgeCoreType = EdgeCore;
        using CoreType = Core;

        CyclicGraphNode() {}

        CyclicGraphNode(Core const& core) :
            Base(core) {}

        // MARK: GraphNode

        typename Base::NodeSharedPtr
        AddEdge(typename Base::NodeSharedPtr toNode, EdgeCore model = EdgeCore()) override {
            PJ::Log("Error. Call CyclicGraph.AddEdge instead");
            return {};
        }
    };
} // namespace PJ
