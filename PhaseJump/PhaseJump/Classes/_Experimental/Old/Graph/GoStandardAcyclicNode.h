#include "String.h"

// namespace PJ
//{
//     namespace Graph
//     {
//         /// <summary>
//         /// Standard node for building complex behavior/algorithms from graph
//         nodes
//         /// </summary>
//         class GoStandardAcyclicNode : public
//         GoAcyclicNode<PJ::Graph::StandardEdgeModel>
//         {
//         public:
//             using Base = GoAcyclicNode<PJ::Graph::StandardEdgeModel>;
//
//             class EdgeType
//             {
//             protected:
//                 EdgeType(String value) { Value = value; }
//
//             public:
//                 String Value { get; private set; }
//
//                 static EdgeType Output { get { return new EdgeType("output");
//                 } } static EdgeType Input { get { return new
//                 EdgeType("input"); } }
//             };
//
//             GoStandardAcyclicNode() : Base()
//             {
//             }
//
//             virtual void AddOutput(String outputId, GoStandardAcyclicNode
//             toNode, String inputId)
//             {
//                 if (null == toNode) { return; }
//
//                 auto tags = MAKE<Tags>();
//                 tags[EdgeType.Output.Value] = outputId;
//                 tags[EdgeType.Input.Value] = inputId;
//
//                 auto outputEdgeModel = new StandardEdgeModel(outputId,
//                 EdgeType.Output.Value, 1.0f, tags); AddEdge(outputEdgeModel,
//                 toNode);
//             }
//
//             virtual GoStandardAcyclicNode OutputNodeFor(String id)
//             {
//                 auto edge = OutputEdgeFor(id);
//                 return null != edge ? edge.toNode.Value as
//                 GoStandardAcyclicNode : null;
//             }
//
//             virtual GoStandardAcyclicNode InputNodeFor(String id)
//             {
//                 for (auto& node : fromNodes)
//                 {
//                     if (node.Reference.TryGetTarget(out
//                     GraphNode<StandardEdgeModel> fromNode))
//                     {
//                         for (Edge fromEdge : fromNode.Edges)
//                         {
//                             if (fromEdge.toNode.Value != this) { continue; }
//                             if (null == fromEdge.model.tags) { continue; }
//
//                             auto inputId =
//                             fromEdge.model.tags[EdgeType.Input.Value] as
//                             String; if (inputId == id)
//                             {
//                                 return fromNode as GoStandardAcyclicNode;
//                             }
//                         }
//                     }
//                 }
//
//                 return null;
//             }
//
//             virtual Edge OutputEdgeFor(String id)
//             {
//                 auto typeEdges = edges.FindAll(edge => edge.model.id == id);
//                 if (typeEdges.Count == 0)
//                 {
//                     return null;
//                 }
//
//                 auto resultEdge = typeEdges[0];
//                 return resultEdge;
//             }
//
//             virtual object ValueForInput<T>(String id, Tags parameters, T
//             defaultValue)
//             {
//                 auto inputNode = InputNodeFor(id);
//                 if (null == inputNode) { return defaultValue; }
//                 return inputNode.ValueForOutput<T>(id, parameters,
//                 defaultValue);
//             }
//
//             virtual object ValueForOutput<T>(String id, Tags parameters, T
//             defaultValue)
//             {
//                 return defaultValue;
//             }
//         };
//     }
// }
