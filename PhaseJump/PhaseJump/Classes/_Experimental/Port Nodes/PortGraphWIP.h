// namespace PJ {
//     namespace PaintEngine {
//         class PenInputPortNode {};
//
//         class Configurator {
//             void Configure() {
//                 auto penInputPortNodeClass = MAKE<PortNodeClass>(PortNodeClass::Type::Data);
//                 penInputPortNodeClass->RegisterOutput<float>("pressure", "Pressure");
//                 penInputPortNodeClass->RegisterOutput<Vector2>("pos", "Position");
//
//                 auto paintActionNodeClass = MAKE<PortNodeClass>(PortNodeClass::Type::Action);
//
//                 // Stroke smoothing is handled outside of the brush node system, because it
//                 requires
//                     // accumulating multiple pen input events and then smoothly interpolating
//                     their values penInputPortNodeClass->RegisterInput<float>("scale", "Scale");
//                 penInputPortNodeClass->RegisterOutput<float>("angle", "Angle");
//             }
//         }
//
//         struct PaintEventCore {
//
//             ScreenPosition position;
//             Angle rotation;
//             float scale;
//         };
//
//         using PaintEvent = Event<PaintEventCore>;
//     } // namespace PaintEngine
//
//     /// Handles logic for evaluating nodes in the port graph
//     class PortGraphRunner {
//         template <class Input>
//         void Run(Input input) {
//             // If we find a node with an action input, start working backwards to the first
//             action and evaluate from there
//
//             //
//             // evaluate based on the input
//         }
//     };
//
//     /// Handles logic for evaluating nodes in the port graph
//     class SomePortGraphProcessor {
//     public:
//         virtual void Process(PortEvaluation eval) = 0;
//     };
//
//     class TestPortGraphHandler : public WorldComponent {
//     public:
//         SP<PortGraphRunner> runner;
//         SP<SomePortGraphProcessor> processor;
//     };
//
//     class TestPortGraphProcessor : public SomePortGraphProcessor {
//     public:
//         void Process(PortEvaluation eval) override {
//             // Get paint events
//
//             // Iterate over events
//
//             // Print position
//         }
//     };
//
//     // support default values for inputs (or the evaluator does this)
//
//     /// A node that use used to process inputs from outputs
//     /// Example: a graph of nodes that produce a GPU shader
//     //    class PortNode
//     //    public:
//     //        virtual void AddOutput(String outputId, SP<PortNode> toNode, String inputId) {
//     //            if (nullptr == toNode) {
//     //                return;
//     //            }
//     //
//     //            AddEdge(toNode, EdgeCore(outputId, inputId));
//     //        }
//     //
//     //        template <class Collection>
//     //        void CollectNodesFromOutput(String outputId, Collection& result) {
//     //            result.clear();
//     //
//     //            List<SP<Edge>> outputEdges;
//     //            CollectEdgesFromOutput(outputId, outputEdges);
//     //
//     //            for (auto& edge : outputEdges) {
//     //                auto toPortNode = SCAST<PortNode>(edge->toNode->Value());
//     //                result.Add(toPortNode);
//     //            }
//     //        }
//     //
//     //        template <class Collection>
//     //        void CollectEdgesFromOutput(String outputId, Collection& result) {
//     //            result.clear();
//     //
//     //            for (auto& edge : edges) {
//     //                auto& edgeCore = edge->core;
//     //                if (edgeCore.IsFromOutput(outputId)) {
//     //                    result.Add(edge);
//     //                }
//     //            }
//     //        }
//     //
//     //        virtual List<SP<Edge>> OutputEdgeList(String id) {
//     //            List<SP<Edge>> result;
//     //            CollectEdgesFromOutput(id, result);
//     //            return result;
//     //        }
//     //
//     //        template <class Collection>
//     //        void CollectNodesToInput(String inputId, Collection& result) {
//     //            for (auto& node : fromNodes) {
//     //                auto nodeLock = node.lock();
//     //                if (!nodeLock) {
//     //                    continue;
//     //                }
//     //                auto fromNode = SCAST<PortNode>(nodeLock);
//     //
//     //                for (auto& fromEdge : fromNode->Edges()) {
//     //                    if (fromEdge->toNode->Value().get() != this) {
//     //                        continue;
//     //                    }
//     //
//     //                    if (fromEdge->core.inputId == inputId) {
//     //                        result.Add(fromNode);
//     //                    }
//     //                }
//     //            }
//     //        }
//     //    };
// } // namespace PJ
