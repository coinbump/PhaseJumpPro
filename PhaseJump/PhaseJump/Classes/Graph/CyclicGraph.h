#ifndef PJCYCLICGRAPH_H
#define PJCYCLICGRAPH_H

#include "_Set.h"
#include "StandardEdgeModel.h"
#include "CyclicGraphNode.h"
#include "WeakReference.h"
#include "Updatable.h"
#include <vector>
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    /// <summary>
    /// Supports a graph with cycles (State machines, story sequences, etc.)
    /// The graph holds on to nodes, and the nodes have weak references to each other
    /// </summary>
    template <class EdgeModel = StandardEdgeModel>
    class CyclicGraph : public Core, public Updatable
    {
    public:
        using EdgeModelSharedPtr = std::shared_ptr<EdgeModel>;
        using Node = CyclicGraphNode<EdgeModel>;
        using NodeWeakPtr = std::weak_ptr<Node>;
        using NodeSharedPtr = std::shared_ptr<Node>;
        using NodePtr = NodeSharedPtr const&;
        using NodeSet = Set<NodeSharedPtr>;

        /// <summary>
        /// Holds strong references to nodes added to graph
        /// </summary>
        NodeSet nodes;

    protected:
        // Optional root node
        NodeWeakPtr rootNode;

    public:
        NodeSharedPtr RootNode() const
        {
            return rootNode.lock();
        }

        void SetRootNode(NodePtr node) {
            rootNode = node;
        }

        void Remove(NodePtr node)
        {
            if (nullptr == node) { return; }

            auto iterFromNodes = node->FromNodes();
            for (auto fromNode : iterFromNodes)
            {
                fromNode.lock()->RemoveEdgesTo(node);
            }

            auto iterEdges = node->Edges();
            for (auto edge : iterEdges)
            {
                node->RemoveEdge(edge);
            }

            if (node == RootNode())
            {
                SetRootNode(NULL);
            }
            nodes.Remove(node);
        }

        void AddEdge(NodePtr fromNode, EdgeModel model, NodeSharedPtr toNode)
        {
            if (NULL == fromNode || NULL == toNode) { return; }

            fromNode->AddEdgeInternal(model, std::make_shared<WeakReference<SomeGraphNode<EdgeModel>>>(toNode));

            nodes.Add(fromNode);
            nodes.Add(toNode);
        }

        virtual void OnUpdate(TimeSlice time) override {
            for (auto node : nodes) {
                node->OnUpdate(time);
            }
        }

        bool IsFinished() const override { return false; }
    };
}

#endif
