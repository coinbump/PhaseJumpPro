#include "CharacterController.h"

using namespace std;
using namespace PJ;

CharacterController::CharacterController() {
    StateMachine<StateType>::OnStateChangeFunc onStateChangeFunc = [this](auto& states) {
        behavior.reset();

        try {
            auto state = states.State();
            auto& buildFunc = buildBehaviorFuncs.at(state);
            behavior = buildFunc(*this);
        } catch (...) {}
    };

    states.OverrideOnStateChangeFunc(onStateChangeFunc);
}

void CharacterController::OnUpdate(TimeSlice time) {
    Base::OnUpdate(time);

    GUARD(behavior)
    behavior->OnUpdate(time);
}

void CharacterController::AddBuildBehaviorFunc(StateType state, BuildBehaviorFunc func) {
    states.Add(state);
    buildBehaviorFuncs.insert_or_assign(state, func);

    GUARD(states.State() == state)
    states.ForceOnStateChange();
}
