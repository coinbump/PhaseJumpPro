#ifndef PJCYCLICGRAPH_H
#define PJCYCLICGRAPH_H

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
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    /// Supports a graph with cycles (State machines, story sequences, etc.)
    /// The graph holds on to nodes, and the nodes have weak references to each
    /// other
    // TODO: re-evaluate use of multiple inheritance here
    template <class Node>
    class CyclicGraph : public Base, public Updatable {
    public:
        using EdgeCore = Node::EdgeModelType;
        using EdgeModelSharedPtr = SP<EdgeCore>;
        using NodeWeakPtr = WP<Node>;
        using NodeSharedPtr = SP<Node>;
        using NodePtr = NodeSharedPtr const&;
        using NodeSet = UnorderedSet<NodeSharedPtr>;

        /// Holds strong references to nodes added to graph
        NodeSet nodes;

    protected:
        // Optional root node
        NodeWeakPtr rootNode;

    public:
        NodeSharedPtr Root() const {
            return rootNode.lock();
        }

        void SetRoot(NodePtr node) {
            rootNode = node;
        }

        void Remove(NodePtr node) {
            if (nullptr == node) {
                return;
            }

            auto iterFromNodes = node->FromNodes();
            for (auto& fromNode : iterFromNodes) {
                fromNode.lock()->RemoveEdgesTo(*node);
            }

            auto iterEdges = node->Edges();
            for (auto& edge : iterEdges) {
                node->RemoveEdge(edge);
            }

            if (node == Root()) {
                SetRoot(nullptr);
            }
            nodes.erase(node);
        }

        void AddEdge(NodePtr fromNode, EdgeCore model, NodeSharedPtr toNode) {
            if (NULL == fromNode || NULL == toNode) {
                return;
            }

            fromNode->AddEdgeInternal(
                model, MAKE<WeakReference<SomeGraphNode<EdgeCore, typename Node::CoreType>>>(toNode)
            );

            nodes.insert(fromNode);
            nodes.insert(toNode);
        }

        virtual void OnUpdate(TimeSlice time) override {
            for (auto& node : nodes) {
                node->OnUpdate(time);
            }
        }
    };
} // namespace PJ

#endif
