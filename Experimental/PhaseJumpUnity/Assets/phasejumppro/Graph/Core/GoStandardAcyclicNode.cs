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
        public class GoStandardAcyclicNode : GoAcyclicGraphNode<PJ.StandardEdgeModel>
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

            public GoStandardAcyclicNode() : base()
            {
            }

            public virtual void AddOutput(string outputId, GoStandardAcyclicNode toNode, string inputId)
            {
                if (null == toNode) { return; }

                var tags = new Tags();
                tags[EdgeType.Output.Value] = outputId;
                tags[EdgeType.Input.Value] = inputId;

                var outputEdgeModel = new StandardEdgeModel(outputId, EdgeType.Output.Value, 1.0f, tags);
                AddEdge(outputEdgeModel, toNode);
            }

            public virtual GoStandardAcyclicNode OutputNodeFor(string id)
            {
                var edge = OutputEdgeFor(id);
                return null != edge ? edge.toNode.Value as GoStandardAcyclicNode : null;
            }

            public virtual GoStandardAcyclicNode InputNodeFor(string id)
            {
                foreach (HashedWeakReference<SomeGraphNode<StandardEdgeModel>> node in fromNodes)
                {
                    if (node.Reference.TryGetTarget(out SomeGraphNode<StandardEdgeModel> fromNode))
                    {
                        foreach (Edge fromEdge in fromNode.Edges)
                        {
                            if (fromEdge.toNode.Value != this) { continue; }
                            if (null == fromEdge.model.tags) { continue; }

                            var inputId = fromEdge.model.tags[EdgeType.Input.Value] as string;
                            if (inputId == id)
                            {
                                return fromNode as GoStandardAcyclicNode;
                            }
                        }
                    }
                }

                return null;
            }

            public virtual Edge OutputEdgeFor(string id)
            {
                var typeEdges = edges.FindAll(edge => edge.model.id == id);
                if (typeEdges.Count == 0)
                {
                    return null;
                }

                var resultEdge = typeEdges[0];
                return resultEdge;
            }

            public virtual object ValueForInput<T>(string id, Tags parameters, T defaultValue)
            {
                var inputNode = InputNodeFor(id);
                if (null == inputNode) { return defaultValue; }
                return inputNode.ValueForOutput<T>(id, parameters, defaultValue);
            }

            public virtual object ValueForOutput<T>(string id, Tags parameters, T defaultValue)
            {
                return defaultValue;
            }
        }
    }
}
