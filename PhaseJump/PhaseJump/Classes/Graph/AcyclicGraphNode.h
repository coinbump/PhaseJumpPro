#ifndef PJACYCLICGRAPHNODE_H
#define PJACYCLICGRAPHNODE_H

#include "SomeGraphNode.h"
#include "StandardEdgeCore.h"
#include "StrongReference.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 5/12/24
 */
namespace PJ {
    /// An acyclic graph node, used for trees, where there are no loops (parents
    /// hold on to children)
    template <class EdgeCore = StandardEdgeCore, class Core = Void>
    class AcyclicGraphNode : public SomeGraphNode<EdgeCore, Core> {
    public:
        using This = AcyclicGraphNode<EdgeCore, Core>;
        using Base = SomeGraphNode<EdgeCore, Core>;
        using AcyclicNode = This;
        using AcyclicNodeSharedPtr = SP<AcyclicNode>;
        using NodeStrongReference = StrongReference<SomeGraphNode<EdgeCore>>;

        typename Base::NodeSharedPtr
        AddEdge(typename Base::NodeSharedPtr toNode, EdgeCore model = EdgeCore()) override {
            // Acyclic graph nodes hold a strong reference to their next node
            this->AddEdgeInternal(model, MAKE<NodeStrongReference>(toNode));
            return toNode;
        }

        AcyclicNodeSharedPtr Root() {
            auto node = SCAST<typename Base::Node>(this->shared_from_this());
            typename Base::NodeSet searchedNodes;

            while (node->FromNodes().size() > 0) {
                // Prevent infinite loop for incorrect graphs
                if (Contains(searchedNodes, node)) {
                    return nullptr;
                }

                searchedNodes.insert(node);

                // A tree node can only have 1 fromNode (the parent)
                auto weakFromNode = *node->FromNodes().begin();
                auto target = weakFromNode.lock();
                if (target) {
                    node = DCAST<This>(target);
                }
            }

            return DCAST<This>(node);
        }

        AcyclicNodeSharedPtr Parent() const {
            GUARDR(!IsEmpty(this->FromNodes()), nullptr);

            // A tree node can only have 1 fromNode (the parent)
            auto iterator = this->FromNodes().begin();
            if (iterator == this->FromNodes().end()) {
                return nullptr;
            }

            auto weakFromNode = *iterator;
            auto target = weakFromNode.lock();
            return DCAST<This>(target);
        }

        bool IsRoot() const {
            return this->FromNodes().size() <= 0;
        }

        void OnUpdate(TimeSlice time) override {
            // TODO: rethink this
            //            if (IsRoot()) {
            //                auto nodes = this->CollectGraph();
            //                for (auto& node : nodes) {
            //                    if (node.get() == this) {
            //                        continue;
            //                    }
            //                    node->OnUpdate(time);
            //                }
            //            }
        }
    };
} // namespace PJ

#endif
