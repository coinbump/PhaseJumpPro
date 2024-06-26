#ifndef PJSOMEGRAPHNODE_H
#define PJSOMEGRAPHNODE_H

#include "Core.h"
#include "Updatable.h"
#include "VectorList.h"
#include "List.h"
#include "_Set.h"
#include "_String.h"
#include "SomeReference.h"
#include "StandardEdgeModel.h"
#include "Tags.h"
#include <memory>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    /// <summary>
    /// Use to build trees, graphs, state machines, etc.
    /// </summary>
    template <class EdgeModel = StandardEdgeModel>
    class SomeGraphNode : public Base, public Updatable
    {
    public:
        using This = SomeGraphNode<EdgeModel>;
        using Node = SomeGraphNode<EdgeModel>;
        using NodeWeakPtr = WP<Node>;
        using NodeSharedPtr = SP<Node>;
        using NodePtr = NodeSharedPtr const&;
        using NodeReference = SomeReference<Node>;
        using NodeReferenceSharedPtr = SP<SomeReference<Node>>;
        using NodeSet = Set<NodeSharedPtr>;
        using WeakNodeVectorList = VectorList<NodeWeakPtr>;
        using NodeVectorList = VectorList<NodeSharedPtr>;

        struct Edge
        {
            NodeWeakPtr fromNode;
            EdgeModel model;
            NodeReferenceSharedPtr toNode;

            Edge(NodeWeakPtr fromNode, EdgeModel model, NodeReferenceSharedPtr toNode) : fromNode(fromNode), model(model), toNode(toNode)
            {
            }
        };

        using EdgeSharedPtr = SP<Edge>;
        using EdgePtr = EdgeSharedPtr const&;
        using EdgeVectorList = VectorList<EdgeSharedPtr>;

        String id;

        virtual ~SomeGraphNode() {}

    protected:
        EdgeVectorList edges;
        WeakNodeVectorList fromNodes;

    public:
        EdgeVectorList& Edges() { return edges; }
        EdgeVectorList const& Edges() const { return edges; }
        std::size_t EdgeCount() { return edges.size(); }
        WeakNodeVectorList& FromNodes() { return fromNodes; }
        WeakNodeVectorList const& FromNodes() const { return fromNodes; }
        String Id() const { return id; }

        SomeGraphNode() {}

        virtual NodeSharedPtr AddEdge(EdgeModel model, NodeSharedPtr toNode) = 0;

        void AddEdgeInternal(EdgeModel model, NodeReferenceSharedPtr toNode)
        {
            if (NULL == toNode || NULL == toNode->Value()) { return; }

            auto fromNode = SCAST<Node>(this->shared_from_this());
            auto forwardEdge = MAKE<Edge>(fromNode, model, toNode);
            edges.Add(forwardEdge);
            toNode->Value()->FromNodes().Add(fromNode);
        }

        void RemoveEdge(EdgePtr _edge)
        {
            if (NULL == _edge) { return; }

            auto edge = _edge;
            edges.Remove(edge);

            if (NULL == edge->toNode->Value()) { return; }

            auto fromNode = this->shared_from_this();
            edge->toNode->Value()->FromNodes().RemoveIf([fromNode](NodeWeakPtr node) -> bool { return node.lock() == fromNode; });
        }

        void RemoveEdgesFrom(NodePtr fromNode)
        {
            if (NULL == fromNode) { return; }
            fromNode->RemoveEdgesTo(SCAST<Node>(this->shared_from_this()));
        }

        void RemoveEdgesTo(NodePtr toNode)
        {
            auto iterEdges = edges;
            for (auto edge : iterEdges)
            {
                if (edge->toNode->Value() == toNode)
                {
                    RemoveEdge(edge);
                }
            }
        }

        void Clear()
        {
            auto iterEdges = EdgeVectorList(edges);
            for (auto edge : iterEdges)
            {
                RemoveEdge(edge);
            }
        }

        /// <summary>
        /// Collect all forward-directed nodes from this node
        /// </summary>
        NodeSet CollectGraph()
        {
            auto fromNode = SCAST<Node>(this->shared_from_this());
            
            NodeSet nodes;
            nodes.Add(fromNode);

            NodeSet searchedNodes;
            searchedNodes.Add(fromNode);

            return CollectEdgeNodesTo(nodes, searchedNodes, true);
        }

        /// <summary>
        /// Collects depth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        /// </summary>
        NodeVectorList CollectDepthFirstGraph()
        {
            auto fromNode = SCAST<Node>(this->shared_from_this());

            NodeVectorList result;
            result.Add(fromNode);

            NodeSet searchedNodes;
            searchedNodes.Add(fromNode);

            CollectDepthFirstChildren(result, fromNode, searchedNodes);
            return result;
        }

        /// <summary>
        /// Collects breadth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        /// </summary>
        NodeVectorList CollectBreadthFirstGraph()
        {
            auto fromNode = SCAST<Node>(this->shared_from_this());

            NodeVectorList result;
            result.Add(fromNode);

            NodeSet searchedNodes;
            searchedNodes.Add(fromNode);

            NodeSet allNodes;
            allNodes.Add(fromNode);

            fromNode->CollectBreadthFirstChildren(result, fromNode, searchedNodes, allNodes);
            return result;
        }

        NodeSet CollectConnectedTo(bool isDeep)
        {
            NodeSet nodes;
            NodeSet searchedNodes;
            return CollectEdgeNodesTo(nodes, searchedNodes, isDeep);
        }

    protected:
        NodeSet CollectEdgeNodesTo(NodeSet& nodes, NodeSet& searchedNodes, bool isDeep)
        {
            auto iterEdges = edges;
            for (auto edge : iterEdges)
            {
                auto toNode = edge->toNode;
                nodes.Add(toNode->Value());

                // Prevent infinite loop
                if (isDeep && !searchedNodes.Contains(toNode->Value()))
                {
                    searchedNodes.Add(toNode->Value());
                    toNode->Value()->CollectEdgeNodesTo(nodes, searchedNodes, true);
                }
            }

            return nodes;
        }

        void CollectBreadthFirstChildren(NodeVectorList& nodes, NodeSharedPtr fromNode, NodeSet& searchedNodes, NodeSet& allNodes)
        {
            for (auto edge : fromNode->Edges())
            {
                auto toNode = edge->toNode;

                // Avoid duplicates for graphs with loops
                if (allNodes.Contains(toNode->Value()))
                {
                    continue;
                }

                allNodes.Add(toNode->Value());
                nodes.Add(toNode->Value());
            }

            for (auto edge : fromNode->Edges())
            {
                auto toNode = edge->toNode;

                // Avoid infinite loop for graphs with loops
                if (searchedNodes.Contains(toNode->Value()))
                {
                    continue;
                }

                searchedNodes.Add(toNode->Value());
                CollectBreadthFirstChildren(nodes, toNode->Value(), searchedNodes, allNodes);
            }
        }

        void CollectDepthFirstChildren(NodeVectorList& nodes, NodeSharedPtr fromNode, NodeSet& searchedNodes)
        {
            if (NULL == fromNode) { return; }

            for (auto edge : fromNode->Edges())
            {
                auto toNode = edge->toNode;

                // Avoid infinite loop for graphs with loops
                if (searchedNodes.Contains(toNode->Value()))
                {
                    continue;
                }

                searchedNodes.Add(toNode->Value());
                nodes.Add(toNode->Value());
                CollectDepthFirstChildren(nodes, toNode->Value(), searchedNodes);
            }
        }

    public:
        void OnUpdate(TimeSlice time) override {}
        bool IsFinished() const override { return false; }
    };
}

#endif
