using System;
using System.Collections.Generic;

namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Direction allows us to use the weak reference in the destination node to refer
        /// back to the parent without creating a circular reference.
        /// </summary>
        public enum Direction
        {
            Forward, Back
        }

        /// <summary>
        /// Specifies how edges in a graph are constructed
        /// </summary>
        public enum EdgeLinkType
        {
            /// <summary>
            /// Edges point forward (state machine)
            /// </summary>
            Forward,

            /// <summary>
            /// Edges point forward and back (trees)
            /// </summary>
            Bidirectional
        }

        // Future: support Connector class type to define inputs, outputs (if needed)
        public class Node<EdgeModel>
        {
            public class Edge
            {
                public WeakReference<Node<EdgeModel>> fromNode { get; set; }
                public EdgeModel model;
                public Node<EdgeModel> toNode;
                public Direction direction;

                // For bidirectional edges, link the  edges together for removal
                public WeakReference<Edge> backEdge { get; set; }

                public Edge(WeakReference<Node<EdgeModel>> fromNode, EdgeModel model, Node<EdgeModel> toNode, Direction direction)
                {
                    this.fromNode = fromNode;
                    this.model = model;
                    this.toNode = toNode;
                    this.direction = direction;
                }
            }

            public string identifier;
            public EdgeLinkType edgeLinkType = EdgeLinkType.Forward;

            /// <summary>
            /// Custom properties
            /// </summary>
            public Tags tags = new Tags();

            /// <summary>
            /// In some graphs, order matters (layers, selectors).
            /// </summary>
            protected List<Edge> edges = new List<Edge>();

            public List<Edge> Edges { get { return edges; } }

            public string Identifier => identifier;

            public void AddEdge(EdgeModel model, Node<EdgeModel> toNode)
            {
                switch (edgeLinkType)
                {
                    case EdgeLinkType.Forward:
                        var forwardEdge = new Edge(new WeakReference<Node<EdgeModel>>(this), model, toNode, Direction.Forward);
                        edges.Add(forwardEdge);
                        break;
                    case EdgeLinkType.Bidirectional:
                        AddBidirectionalEdges(model, toNode, model);
                        break;
                }
            }

            public void AddBidirectionalEdges(EdgeModel forwardModel, Node<EdgeModel> toNode, EdgeModel backModel)
            {
                var forwardEdge = new Edge(new WeakReference<Node<EdgeModel>>(this), forwardModel, toNode, Direction.Forward);
                edges.Add(forwardEdge);
                var backEdge = new Edge(new WeakReference<Node<EdgeModel>>(this), backModel, toNode, Direction.Back);
                toNode.edges.Add(backEdge);

                forwardEdge.backEdge = new WeakReference<Edge>(backEdge);
                backEdge.backEdge = new WeakReference<Edge>(forwardEdge);
            }

            public void RemoveEdge(Edge edge)
            {
                if (!edges.Contains(edge)) { return; }

                edges.Remove(edge);

                if (null != edge.backEdge && edge.backEdge.TryGetTarget(out Edge backEdge))
                {
                    switch (backEdge.direction)
                    {
                        case Direction.Forward:
                            if (edge.fromNode.TryGetTarget(out Node<EdgeModel> fromNode))
                            {
                                fromNode.RemoveEdge(backEdge);
                            }
                            break;
                        case Direction.Back:
                            edge.toNode.RemoveEdge(backEdge);
                            break;
                    }
                }
            }

            public void RemoveEdgesFrom(Node<EdgeModel> fromNode)
            {
                var iterEdges = new List<Edge>(edges);
                foreach (Edge edge in iterEdges)
                {
                    if (edge.fromNode.TryGetTarget(out Node<EdgeModel> iterFromNode))
                    {
                        if (iterFromNode == fromNode)
                        {
                            RemoveEdge(edge);
                        }
                    }
                    else
                    {
                        // Prune edges to deallocated nodes
                        edges.Remove(edge);
                    }
                }
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

                return CollectEdgeNodes(nodes, searchedNodes, Direction.Forward, true);
            }

            public HashSet<Node<EdgeModel>> CollectConnected(Direction direction, bool isDeep)
            {
                var nodes = new HashSet<Node<EdgeModel>>();
                var searchedNodes = new HashSet<Node<EdgeModel>>();
                return CollectEdgeNodes(nodes, searchedNodes, Direction.Forward, isDeep);
            }

            protected HashSet<Node<EdgeModel>> CollectEdgeNodes(HashSet<Node<EdgeModel>> nodes, HashSet<Node<EdgeModel>> searchedNodes, Direction direction, bool isDeep)
            {
                var iterEdges = new List<Edge>(edges);
                foreach (Edge edge in iterEdges)
                {
                    if (edge.direction == direction)
                    {
                        nodes.Add(edge.toNode);

                        // Prevent infinite loop
                        if (isDeep && !searchedNodes.Contains(edge.toNode)) {
                           searchedNodes.Add(edge.toNode);
                           edge.toNode.CollectEdgeNodes(nodes, searchedNodes, direction, true);
                        }
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
