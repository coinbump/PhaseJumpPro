#pragma once

#include "CyclicGraph.h"
#include "CyclicGraphNode.h"
#include "PortEdgeCore.h"
#include "PortEvaluation.h"
#include "PortNodeClass.h"
#include "Tags.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// A graph node with input and output ports
    /// The node can be evaluated, which produces an evaluation output
    class PortNode : public CyclicGraphNode<PortEdgeCore> {
    public:
        using Base = CyclicGraphNode<PortEdgeCore>;
        using This = PortNode;
        using EdgeCore = PortEdgeCore;

        SP<PortNodeClass> _class;
        Tags values;
        std::optional<PortNodeClass::Type> type;

        /// Stores most recent evaluation (used as a cache if the node is only evaluated once)
        SP<PortEvaluation> evaluation;

        PortNode() {}

        PortNodeClass::Type ClassType() const {
            GUARDR(!type, type.value())
            GUARDR(!_class, _class->type);

            return PortNodeClass::Type::Data;
        }
    };

    class PortGraph : public CyclicGraph<PortNode> {
    public:
        using EdgeCore = PortEdgeCore;
    };
} // namespace PJ
