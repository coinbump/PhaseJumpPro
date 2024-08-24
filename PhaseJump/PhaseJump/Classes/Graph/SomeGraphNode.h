#pragma once

#include "_String.h"
#include "List.h"
#include "OrderedSet.h"
#include "SomeReference.h"
#include "StandardCore.h"
#include "StandardEdgeCore.h"
#include "Tags.h"
#include "Updatable.h"
#include "VectorList.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/23
 */
namespace PJ {
    /// Use to build trees, graphs, state machines, etc.
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class SomeGraphNode : public OwnerBase<Core>, public Updatable {
    public:
        using Base = OwnerBase<Core>;
        using This = SomeGraphNode<EdgeCore, Core>;
        using Node = This;
        using NodeWeakPtr = WP<Node>;
        using NodeSharedPtr = SP<Node>;
        using NodeReference = SomeReference<Node>;
        using NodeReferenceSharedPtr = SP<SomeReference<Node>>;
        // TODO: why is this set ordered?
        using NodeSet = OrderedSet<NodeSharedPtr>;
        using WeakNodeSet = OrderedSet<NodeWeakPtr, std::owner_less<NodeWeakPtr>>;
        using NodeList = VectorList<NodeSharedPtr>;

        struct Edge {
            // TODO: doesn't make sense
            NodeWeakPtr fromNode;
            EdgeCore core;
            NodeReferenceSharedPtr toNode;

            Edge(NodeWeakPtr fromNode, EdgeCore model, NodeReferenceSharedPtr toNode) :
                fromNode(fromNode),
                core(model),
                toNode(toNode) {}
        };

        using EdgeSharedPtr = SP<Edge>;
        using EdgePtr = EdgeSharedPtr const&;
        using EdgeList = VectorList<EdgeSharedPtr>;

        String id;

        virtual ~SomeGraphNode() {}

    protected:
        EdgeList edges;
        // TODO: why weak pointers? When a node is removed from the graph, it should remove all
        // references to itself, so...
        WeakNodeSet fromNodes;

    public:
        EdgeList& Edges() {
            return edges;
        }

        EdgeList const& Edges() const {
            return edges;
        }

        std::size_t EdgeCount() {
            return edges.size();
        }

        WeakNodeSet& FromNodes() {
            return fromNodes;
        }

        WeakNodeSet const& FromNodes() const {
            return fromNodes;
        }

        String Id() const {
            return id;
        }

        SomeGraphNode() {}

        SomeGraphNode(Core core) :
            Base(core) {}

        virtual NodeSharedPtr AddEdge(NodeSharedPtr toNode, EdgeCore model = EdgeCore()) = 0;

        void AddEdgeInternal(EdgeCore model, NodeReferenceSharedPtr toNode) {
            if (nullptr == toNode || nullptr == toNode->Value()) {
                return;
            }

            auto fromNode = SCAST<Node>(this->shared_from_this());
            auto forwardEdge = MAKE<Edge>(fromNode, model, toNode);
            Add(edges, forwardEdge);
            toNode->Value()->FromNodes().insert(fromNode);
        }

        void RemoveEdge(EdgePtr _edge) {
            if (nullptr == _edge) {
                return;
            }

            auto edge = _edge;
            Remove(edges, edge);

            if (nullptr == edge->toNode->Value()) {
                return;
            }

            NodeWeakPtr fromNode = SCAST<Node>(this->shared_from_this());
            edge->toNode->Value()->FromNodes().erase(fromNode);
        }

        void RemoveEdgesFrom(Node& fromNode) {
            fromNode.RemoveEdgesTo(*this);
        }

        void RemoveEdgesTo(Node& toNode) {
            auto iterEdges = edges;
            for (auto& edge : iterEdges) {
                auto edgeValue = edge->toNode->Value();
                if (edgeValue.get() == &toNode) {
                    RemoveEdge(edge);
                }
            }
        }

        void RemoveAllEdges() {
            auto iterEdges = EdgeList(edges);
            for (auto& edge : iterEdges) {
                RemoveEdge(edge);
            }
        }

    public:
        void OnUpdate(TimeSlice time) override {}
    };
} // namespace PJ
