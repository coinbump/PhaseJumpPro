#pragma once

#include "List.h"
#include "SomeReference.h"
#include "StandardCore.h"
#include "StandardEdgeCore.h"
#include "StringUtils.h"
#include "Tags.h"
#include "UnorderedSet.h"
#include "Updatable.h"
#include "VectorList.h"
#include "Void.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/21/24
 */
namespace PJ {
    /**
     Use to build trees, graphs, state machines with edges.

     Don't use this for simple trees. Use TreeNode/Treeable for that. The purpose of this class is
     to allow you to attach properties to graph edges
     */
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class SomeGraphNode : public Base {
    public:
        using This = SomeGraphNode<EdgeCore, Core>;
        using Node = This;
        using NodeSharedPtr = SP<Node>;
        using NodeReference = SomeReference<Node>;
        using NodeSet = UnorderedSet<NodeSharedPtr>;
        using WeakNodeSet = UnorderedSet<Node*>;
        using NodeList = VectorList<NodeSharedPtr>;

        struct Edge {
            Node* fromNode{};
            EdgeCore core;
            UP<SomeReference<Node>> toNode;

            Edge(Node* fromNode, EdgeCore model, UP<SomeReference<Node>>& toNode) :
                fromNode(fromNode),
                core(model),
                toNode(std::move(toNode)) {}
        };

        using EdgeList = VectorList<UP<Edge>>;

        String id;
        Core core{};

        virtual ~SomeGraphNode() {}

    protected:
        EdgeList edges;
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
            core(core) {}

        virtual NodeSharedPtr AddEdge(NodeSharedPtr toNode, EdgeCore model = EdgeCore()) = 0;

        void AddEdgeInternal(EdgeCore model, UP<SomeReference<Node>>& toNode) {
            if (nullptr == toNode || nullptr == toNode->Value()) {
                return;
            }

            toNode->Value()->FromNodes().insert(this);
            auto forwardEdge = NEW<Edge>(this, model, toNode);
            edges.push_back(std::move(forwardEdge));
        }

        void AddEdgeInternal(EdgeCore model, UP<SomeReference<Node>>&& toNode) {
            if (nullptr == toNode || nullptr == toNode->Value()) {
                return;
            }

            toNode->Value()->FromNodes().insert(this);
            auto forwardEdge = NEW<Edge>(this, model, toNode);
            edges.push_back(std::move(forwardEdge));
        }

        void RemoveEdge(Edge& _edge) {
            if (_edge.toNode && _edge.toNode->Value()) {
                _edge.toNode->Value()->FromNodes().erase(this);
            }

            RemoveFirstIf(edges, [&](auto& edge) { return edge.get() == &_edge; });
        }

        void RemoveEdgesFrom(Node& fromNode) {
            fromNode.RemoveEdgesTo(*this);
        }

        void RemoveEdgesTo(Node& toNode) {
            VectorList<Edge*> iterEdges = Map<Edge*>(edges, [](auto& edge) { return edge.get(); });
            for (auto& edge : iterEdges) {
                auto edgeValue = edge->toNode->Value();
                if (edgeValue.get() == &toNode) {
                    RemoveEdge(*edge);
                }
            }
        }

        void RemoveAllEdges() {
            VectorList<Edge*> iterEdges = Map<Edge*>(edges, [](auto& edge) { return edge.get(); });
            for (auto& edge : iterEdges) {
                RemoveEdge(*edge);
            }
        }
    };
} // namespace PJ
