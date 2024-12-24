#pragma once

#include "StandardCore.h"
#include "StateMachine.h"

/*
 RATING: 5 stars
 Simple type
 CODE REVIEW: 12/16/24
 */
namespace PJ {
    /// Combines standard core with a state machine
    template <class StateType>
    class StateCore {
    public:
        StandardCore sc;
        StateMachine<StateType> states;
    };
} // namespace PJ
