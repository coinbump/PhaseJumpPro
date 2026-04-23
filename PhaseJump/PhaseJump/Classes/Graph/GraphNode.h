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
#include <algorithm>
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
    class GraphNode : public Base {
    public:
        using This = GraphNode<EdgeCore, Core>;
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

        virtual ~GraphNode() {}

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

        GraphNode() {}

        GraphNode(Core const& core) :
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
            Node* targetNode =
                (_edge.toNode && _edge.toNode->Value()) ? _edge.toNode->Value().get() : nullptr;

            RemoveFirstIf(edges, [&](auto& edge) { return edge.get() == &_edge; });

            // Only drop the backlink when no other edge from this still points to targetNode.
            // Multigraphs can have multiple edges between the same pair; clearing fromNodes
            // unconditionally would leave the surviving edges with a stale backlink.
            GUARD(targetNode)
            for (auto& edge : edges) {
                if (edge && edge->toNode && edge->toNode->Value().get() == targetNode) {
                    return;
                }
            }
            targetNode->FromNodes().erase(this);
        }

        void RemoveEdgesFrom(Node& fromNode) {
            fromNode.RemoveEdgesTo(*this);
        }

        void RemoveEdgesTo(Node& toNode) {
            // Single pass: drop every edge pointing at toNode, then clear the backlink once.
            // Avoids the O(E_out^2) cost of calling RemoveEdge per match.
            auto newEnd = std::remove_if(edges.begin(), edges.end(), [&](auto const& edge) {
                return edge && edge->toNode && edge->toNode->Value().get() == &toNode;
            });
            GUARD(newEnd != edges.end())
            edges.erase(newEnd, edges.end());
            toNode.FromNodes().erase(this);
        }

        void RemoveAllEdges() {
            // Collect distinct target nodes so each backlink is cleared exactly once,
            // then drop all edges in a single swap. O(E_out) instead of O(E_out^2).
            WeakNodeSet targets;
            for (auto& edge : edges) {
                if (edge && edge->toNode && edge->toNode->Value()) {
                    targets.insert(edge->toNode->Value().get());
                }
            }
            edges.clear();
            for (auto* target : targets) {
                target->FromNodes().erase(this);
            }
        }
    };
} // namespace PJ
