#pragma once

#include "_String.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 7/21/24
 */
namespace PJ {
    /// Edge model for port node
    struct PortEdgeCore {
        /// Id of the output port this edge is connected to
        String fromPortId;

        /// Id of the input port this edge is connected to
        String toPortId;

        PortEdgeCore(String fromPortId, String toPortId) :
            fromPortId(fromPortId),
            toPortId(toPortId) {}
    };
} // namespace PJ
