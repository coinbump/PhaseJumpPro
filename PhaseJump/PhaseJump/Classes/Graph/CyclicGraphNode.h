#ifndef PJCYCLICGRAPHNODE_H
#define PJCYCLICGRAPHNODE_H

#include "SomeGraphNode.h"
#include <iostream>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    /// <summary>
    /// Cyclic nodes are managed by CyclicGraph
    /// </summary>
    template <class EdgeModel = StandardEdgeModel>
    class CyclicGraphNode : public SomeGraphNode<EdgeModel>
    {
    public:
        using Base = SomeGraphNode<EdgeModel>;

        void AddEdge(EdgeModel model, typename Base::NodeSharedPtr toNode) override
        {
            std::cout << "Error. Call CyclicGraph.AddEdge instead";
        }
    };
}

#endif
