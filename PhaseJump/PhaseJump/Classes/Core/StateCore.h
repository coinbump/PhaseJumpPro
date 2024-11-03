#pragma once

#include "StandardCore.h"
#include "StateMachine.h"

// /23
namespace PJ {
    /// Aggregates standard core with a state machine
    template <class StateType>
    class StateCore {
    public:
        StandardCore sc;
        StateMachine<StateType> states;
    };
} // namespace PJ
