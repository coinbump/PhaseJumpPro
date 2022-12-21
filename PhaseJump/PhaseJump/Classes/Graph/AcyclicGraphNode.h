#ifndef PJACYCLICGRAPHNODE_H
#define PJACYCLICGRAPHNODE_H

#include "SomeGraphNode.h"
#include "StrongReference.h"
#include "StandardEdgeModel.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/11/22
 */
namespace PJ
{
    template <class EdgeModel = StandardEdgeModel>
    class AcyclicGraphNode : public SomeGraphNode<EdgeModel>
    {
    public:
        using This = AcyclicGraphNode;
        using Base = SomeGraphNode<EdgeModel>;
        using AcyclicNode = AcyclicGraphNode<EdgeModel>;
        using AcyclicNodeSharedPtr = std::shared_ptr<AcyclicNode>;
        using NodeStrongReference = StrongReference<SomeGraphNode<EdgeModel>>;

        void AddEdge(EdgeModel model, typename Base::NodeSharedPtr toNode) override {
            // Acyclic graph nodes hold a strong reference to their next node
            this->AddEdgeInternal(model, std::make_shared<StrongReference<SomeGraphNode<EdgeModel>>>(toNode));
        }

        AcyclicNodeSharedPtr RootNode() {
            auto node = std::static_pointer_cast<typename Base::Node>(this->shared_from_this());
            typename Base::NodeSet searchedNodes;

            while (node->FromNodes().size() > 0)
            {
                searchedNodes.Add(node);

                for (auto weakFromNode : node->FromNodes())
                {
                    auto target = weakFromNode.lock();
                    if (target)
                    {
                        node = std::dynamic_pointer_cast<This>(target);
                        break;
                    }
                }

                // Graph shouldn't be cyclic, but prevent the edge case of an infinite loop
                if (searchedNodes.Contains(node))
                {
                    return NULL;
                }
            }

            return std::dynamic_pointer_cast<This>(node);
        }

        AcyclicNodeSharedPtr Parent() const
        {
            for (auto weakFromNode : this->FromNodes())
            {
                auto target = weakFromNode.lock();
                return std::dynamic_pointer_cast<This>(target);
            }

            return NULL;
        }

        bool IsRootNode() const
        {
            return this->FromNodes().size() <= 0;
        }

        void OnUpdate(TimeSlice time) override
        {
            if (IsRootNode()) {
                auto nodes = this->CollectGraph();
                for (auto node : nodes)
                {
                    if (node.get() == this) { continue; }
                    node->OnUpdate(time);
                }
            }
        }
    };
}

#endif
