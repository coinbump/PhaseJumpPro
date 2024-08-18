#pragma once

#include "StandardCore.h"
#include "StateMachine.h"

namespace PJ {
    /// Aggregates standard core with a state machine
    template <class StateType>
    class StateCore {
    public:
        StandardCore sc;
        StateMachine<StateType> sm;
    };
} // namespace PJ
