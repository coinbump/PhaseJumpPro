using UnityEngine;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 3/12/22
 */
namespace PJ
{
    namespace Graph
    {
        /// <summary>
        /// Standard node for building complex behavior/algorithms from graph nodes
        /// </summary>
        public class GoStandardNode : GoNode<PJ.Graph.StandardEdgeModel>
        {
            // FUTURE: Evaluate Enumeration as an alternative:
            // https://docs.microsoft.com/en-us/dotnet/architecture/microservices/microservice-ddd-cqrs-patterns/enumeration-classes-over-enum-types?
            public class EdgeType
            {
                private EdgeType(string value) { Value = value; }

                public string Value { get; private set; }

                public static EdgeType Output { get { return new EdgeType("output"); } }
                public static EdgeType Input { get { return new EdgeType("input"); } }
            }

            public GoStandardNode() : base()
            {
            }

            public virtual void AddOutput(string outputIdentifier, GoStandardNode toNode, string inputIdentifier)
            {
                if (null == toNode) { return; }

                var tags = new Tags();
                tags[EdgeType.Output.Value] = outputIdentifier;
                tags[EdgeType.Input.Value] = inputIdentifier;

                var outputEdgeModel = new StandardEdgeModel(outputIdentifier, EdgeType.Output.Value, 1.0f, tags);
                AddEdge(outputEdgeModel, toNode);
            }

            public virtual GoStandardNode OutputNodeFor(string identifier)
            {
                var edge = OutputEdgeFor(identifier);
                return null != edge ? edge.toNode as GoStandardNode : null;
            }

            public virtual GoStandardNode InputNodeFor(string identifier)
            {
                foreach (HashedWeakReference<Node<StandardEdgeModel>> node in fromNodes)
                {
                    if (node.Reference.TryGetTarget(out Node<StandardEdgeModel> fromNode))
                    {
                        foreach (Edge fromEdge in fromNode.Edges)
                        {
                            if (fromEdge.toNode != this) { continue; }
                            if (null == fromEdge.model.tags) { continue; }

                            var inputIdentifier = fromEdge.model.tags[EdgeType.Input.Value] as string;
                            if (inputIdentifier == identifier)
                            {
                                return fromNode as GoStandardNode;
                            }
                        }
                    }
                }

                return null;
            }

            public virtual Edge OutputEdgeFor(string identifier)
            {
                var typeEdges = edges.FindAll(edge => edge.model.identifier == identifier);
                if (typeEdges.Count == 0)
                {
                    return null;
                }

                var resultEdge = typeEdges[0];
                return resultEdge;
            }

            public virtual object ValueForInput<T>(string identifier, Tags parameters, T defaultValue)
            {
                var inputNode = InputNodeFor(identifier);
                if (null == inputNode) { return defaultValue; }
                return inputNode.ValueForOutput<T>(identifier, parameters, defaultValue);
            }

            public virtual object ValueForOutput<T>(string identifier, Tags parameters, T defaultValue)
            {
                return defaultValue;
            }
        }
    }
}
