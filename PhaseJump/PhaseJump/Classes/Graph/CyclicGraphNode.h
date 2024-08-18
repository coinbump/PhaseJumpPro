#ifndef PJCYCLICGRAPHNODE_H
#define PJCYCLICGRAPHNODE_H

#include "SomeGraphNode.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    /// Cyclic nodes are managed by CyclicGraph
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class CyclicGraphNode : public SomeGraphNode<EdgeCore, Core> {
    public:
        using Base = SomeGraphNode<EdgeCore, Core>;
        using EdgeModelType = EdgeCore;
        using CoreType = Core;

        CyclicGraphNode() {}

        CyclicGraphNode(Core core) :
            Base(core) {}

        typename Base::NodeSharedPtr
        AddEdge(typename Base::NodeSharedPtr toNode, EdgeCore model = EdgeCore()) override {
            std::cout << "Error. Call CyclicGraph.AddEdge instead";
            return nullptr;
        }
    };
} // namespace PJ

#endif
