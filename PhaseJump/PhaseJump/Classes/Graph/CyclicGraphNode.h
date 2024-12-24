#pragma once

#include "SomeGraphNode.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /// Cyclic nodes are managed by CyclicGraph
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class CyclicGraphNode : public SomeGraphNode<EdgeCore, Core> {
    public:
        using Base = SomeGraphNode<EdgeCore, Core>;
        using EdgeCoreType = EdgeCore;
        using CoreType = Core;

        CyclicGraphNode() {}

        CyclicGraphNode(Core core) :
            Base(core) {}

        // MARK: SomeGraphNode

        typename Base::NodeSharedPtr
        AddEdge(typename Base::NodeSharedPtr toNode, EdgeCore model = EdgeCore()) override {
            PJ::Log("Error. Call CyclicGraph.AddEdge instead");
            return {};
        }
    };
} // namespace PJ
