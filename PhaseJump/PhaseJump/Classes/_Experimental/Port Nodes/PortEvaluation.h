#pragma once

#include "Tags.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/6/25
 */
namespace PJ {
    /// Result produced by evaluating a port graph node
    /// Outputs can be attributes or events
    class PortEvaluation {
    public:
        Tags values;
    };
} // namespace PJ
