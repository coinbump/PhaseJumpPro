using System;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Standard model, with unit tests
 * CODE REVIEW: 3/8/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Edge direction
        /// </summary>
        public enum Direction
        {
            Forward, Back
        }

        /// <summary>
        /// Use to build trees, graphs, state machines, etc.
        /// </summary>
        /// <typeparam name="EdgeModel"></typeparam>
        public class Node<EdgeModel>
        {
            public class Edge
            {
                public WeakReference<Node<EdgeModel>> fromNode { get; set; }
                public EdgeModel model;
                public Node<EdgeModel> toNode;

                public Edge(WeakReference<Node<EdgeModel>> fromNode, EdgeModel model, Node<EdgeModel> toNode)
                {
                    this.fromNode = fromNode;
                    this.model = model;
                    this.toNode = toNode;
                }
            }

            public string identifier;

            /// <summary>
            /// Custom properties
            /// </summary>
            public Tags tags = new Tags();

            /// <summary>
            /// In some graphs, order matters (layers, selectors).
            /// </summary>
            protected List<Edge> edges = new List<Edge>();
            protected HashSet<HashedWeakReference<Node<EdgeModel>>> fromNodes = new HashSet<HashedWeakReference<Node<EdgeModel>>>();
            protected HashedWeakReference<Node<EdgeModel>> WeakThis { get { return new HashedWeakReference<Node<EdgeModel>>(this); } }

            public List<Edge> Edges { get { return edges; } }
            public HashSet<HashedWeakReference<Node<EdgeModel>>> FromNodes { get { return fromNodes; } }

            public string Identifier => identifier;

            public void AddEdge(EdgeModel model, Node<EdgeModel> toNode)
            {
                var forwardEdge = new Edge(new WeakReference<Node<EdgeModel>>(this), model, toNode);
                edges.Add(forwardEdge);
                toNode.FromNodes.Add(new HashedWeakReference<Node<EdgeModel>>(this));
            }

            public void RemoveEdge(Edge edge)
            {
                edges.Remove(edge);
                edge.toNode.FromNodes.Remove(WeakThis);
            }

            public void RemoveEdgesFrom(Node<EdgeModel> fromNode)
            {
                fromNode.RemoveEdgesTo(this);
            }

            public void RemoveEdgesTo(Node<EdgeModel> toNode)
            {
                var iterEdges = new List<Edge>(edges);
                foreach (Edge edge in iterEdges)
                {
                    if (edge.toNode == toNode)
                    {
                        RemoveEdge(edge);
                    }
                }
            }

            public void Clear()
            {
                var iterEdges = new List<Edge>(edges);
                foreach (Edge edge in iterEdges)
                {
                    RemoveEdge(edge);
                }
            }

            /// <summary>
            /// Collect all forward-directed nodes from this node
            /// </summary>
            public HashSet<Node<EdgeModel>> CollectGraph()
            {
                var nodes = new HashSet<Node<EdgeModel>>();
                nodes.Add(this);

                var searchedNodes = new HashSet<Node<EdgeModel>>();
                nodes.Add(this);

                return CollectEdgeNodesTo(nodes, searchedNodes, true);
            }

            public HashSet<Node<EdgeModel>> CollectConnectedTo(bool isDeep)
            {
                var nodes = new HashSet<Node<EdgeModel>>();
                var searchedNodes = new HashSet<Node<EdgeModel>>();
                return CollectEdgeNodesTo(nodes, searchedNodes, isDeep);
            }

            protected HashSet<Node<EdgeModel>> CollectEdgeNodesTo(HashSet<Node<EdgeModel>> nodes, HashSet<Node<EdgeModel>> searchedNodes, bool isDeep)
            {
                var iterEdges = new List<Edge>(edges);
                foreach (Edge edge in iterEdges)
                {
                    nodes.Add(edge.toNode);

                    // Prevent infinite loop
                    if (isDeep && !searchedNodes.Contains(edge.toNode)) {
                        searchedNodes.Add(edge.toNode);
                        edge.toNode.CollectEdgeNodesTo(nodes, searchedNodes, true);
                    }
                }

                return nodes;
            }

            /// <summary>
            /// Call for root node, to update entire graph
            /// </summary>
            /// <param name="time"></param>
            public virtual void EvtUpdateRoot(TimeSlice time)
            {
                var nodes = CollectGraph();
                foreach (Node<EdgeModel> node in nodes)
                {
                    node.EvtUpdate(time);
                }
            }

            /// <summary>
            /// Override if graph node needs to respond to time updates
            /// </summary>
            /// <param name="time"></param>
            public virtual void EvtUpdate(TimeSlice time)
            {
            }
        }
    }
}
