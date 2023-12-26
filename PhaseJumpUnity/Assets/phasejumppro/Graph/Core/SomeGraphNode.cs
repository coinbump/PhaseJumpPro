using System;
using System.Collections.Generic;

/*
 * RATING: 4 stars
 * Standard model, with unit tests
 * CODE REVIEW: 3/8/22
 */
namespace PJ
{
    /// <summary>
    /// Use to build trees, graphs, state machines, etc.
    /// </summary>
    /// <typeparam name="EdgeModel"></typeparam>
    public abstract class SomeGraphNode<EdgeModel> : Core, Updatable
    {
        public class Edge
        {
            public WeakReference<SomeGraphNode<EdgeModel>> fromNode { get; set; }
            public EdgeModel model;
            public SomeReference<SomeGraphNode<EdgeModel>> toNode;

            public Edge(WeakReference<SomeGraphNode<EdgeModel>> fromNode, EdgeModel model, SomeReference<SomeGraphNode<EdgeModel>> toNode)
            {
                this.fromNode = fromNode;
                this.model = model;
                this.toNode = toNode;
            }
        }

        public string id;

        /// <summary>
        /// In some graphs, order matters (layers, selectors).
        /// </summary>
        protected List<Edge> edges = new List<Edge>();
        protected HashSet<HashedWeakReference<SomeGraphNode<EdgeModel>>> fromNodes = new HashSet<HashedWeakReference<SomeGraphNode<EdgeModel>>>();
        protected HashedWeakReference<SomeGraphNode<EdgeModel>> WeakThis { get { return new HashedWeakReference<SomeGraphNode<EdgeModel>>(this); } }

        public List<Edge> Edges { get { return edges; } }
        public HashSet<HashedWeakReference<SomeGraphNode<EdgeModel>>> FromNodes { get { return fromNodes; } }
        public bool IsFinished { get => false; }

        public string Id => id;

        public abstract void AddEdge(EdgeModel model, SomeGraphNode<EdgeModel> toNode);

        public void AddEdgeInternal(EdgeModel model, SomeReference<SomeGraphNode<EdgeModel>> toNode)
        {
            var forwardEdge = new Edge(new WeakReference<SomeGraphNode<EdgeModel>>(this), model, toNode);
            edges.Add(forwardEdge);
            toNode.Value.FromNodes.Add(new HashedWeakReference<SomeGraphNode<EdgeModel>>(this));
        }

        public void RemoveEdge(Edge edge)
        {
            edges.Remove(edge);
            edge.toNode.Value.FromNodes.Remove(WeakThis);
        }

        public void RemoveEdgesFrom(SomeGraphNode<EdgeModel> fromNode)
        {
            fromNode.RemoveEdgesTo(this);
        }

        public void RemoveEdgesTo(SomeGraphNode<EdgeModel> toNode)
        {
            var iterEdges = new List<Edge>(edges);
            foreach (Edge edge in iterEdges)
            {
                if (edge.toNode.Value == toNode)
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
        public HashSet<SomeGraphNode<EdgeModel>> CollectGraph()
        {
            var nodes = new HashSet<SomeGraphNode<EdgeModel>>();
            nodes.Add(this);

            var searchedNodes = new HashSet<SomeGraphNode<EdgeModel>>();
            searchedNodes.Add(this);

            return CollectEdgeNodesTo(nodes, searchedNodes, true);
        }

        /// <summary>
        /// Collects depth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        /// </summary>
        public List<SomeGraphNode<EdgeModel>> CollectDepthFirstGraph()
        {
            var result = new List<SomeGraphNode<EdgeModel>>();
            result.Add(this);

            var searchedNodes = new HashSet<SomeGraphNode<EdgeModel>>();
            searchedNodes.Add(this);

            CollectDepthFirstChildren(result, this, searchedNodes);
            return result;
        }

        protected void CollectDepthFirstChildren(List<SomeGraphNode<EdgeModel>> nodes, SomeGraphNode<EdgeModel> fromNode, HashSet<SomeGraphNode<EdgeModel>> searchedNodes)
        {
            foreach (Edge edge in fromNode.edges)
            {
                var toNode = edge.toNode.Value;

                // Avoid infinite loop for graphs with loops
                if (searchedNodes.Contains(edge.toNode.Value))
                {
                    continue;
                }

                searchedNodes.Add(toNode);
                nodes.Add(toNode);
                CollectDepthFirstChildren(nodes, toNode, searchedNodes);
            }
        }

        /// <summary>
        /// Collects breadth-first ordered graph
        /// NOTE: may not work as expected when loops exist in the graph
        /// </summary>
        public List<SomeGraphNode<EdgeModel>> CollectBreadthFirstGraph()
        {
            var result = new List<SomeGraphNode<EdgeModel>>();
            result.Add(this);

            var searchedNodes = new HashSet<SomeGraphNode<EdgeModel>>();
            searchedNodes.Add(this);

            var allNodes = new HashSet<SomeGraphNode<EdgeModel>>();
            allNodes.Add(this);

            CollectBreadthFirstChildren(result, this, searchedNodes, allNodes);
            return result;
        }

        protected void CollectBreadthFirstChildren(List<SomeGraphNode<EdgeModel>> nodes, SomeGraphNode<EdgeModel> fromNode, HashSet<SomeGraphNode<EdgeModel>> searchedNodes, HashSet<SomeGraphNode<EdgeModel>> allNodes)
        {
            foreach (Edge edge in fromNode.edges)
            {
                var toNode = edge.toNode.Value;

                // Avoid duplicates for graphs with loops
                if (allNodes.Contains(toNode))
                {
                    continue;
                }

                allNodes.Add(toNode);
                nodes.Add(toNode);
            }

            foreach (Edge edge in fromNode.edges)
            {
                var toNode = edge.toNode.Value;

                // Avoid infinite loop for graphs with loops
                if (searchedNodes.Contains(toNode))
                {
                    continue;
                }

                searchedNodes.Add(toNode);
                CollectBreadthFirstChildren(nodes, toNode, searchedNodes, allNodes);
            }
        }

        public HashSet<SomeGraphNode<EdgeModel>> CollectConnectedTo(bool isDeep)
        {
            var nodes = new HashSet<SomeGraphNode<EdgeModel>>();
            var searchedNodes = new HashSet<SomeGraphNode<EdgeModel>>();
            return CollectEdgeNodesTo(nodes, searchedNodes, isDeep);
        }

        protected HashSet<SomeGraphNode<EdgeModel>> CollectEdgeNodesTo(HashSet<SomeGraphNode<EdgeModel>> nodes, HashSet<SomeGraphNode<EdgeModel>> searchedNodes, bool isDeep)
        {
            var iterEdges = new List<Edge>(edges);
            foreach (Edge edge in iterEdges)
            {
                var toNode = edge.toNode.Value;
                nodes.Add(toNode);

                // Prevent infinite loop
                if (isDeep && !searchedNodes.Contains(toNode))
                {
                    searchedNodes.Add(toNode);
                    toNode.CollectEdgeNodesTo(nodes, searchedNodes, true);
                }
            }

            return nodes;
        }

        public virtual void OnUpdate(TimeSlice time)
        {
        }
    }
}
