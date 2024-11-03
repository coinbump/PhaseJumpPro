#include "ComponentsController.h"
#include "WorldNode.h"

using namespace std;
using namespace PJ;

ComponentsController::ComponentsController() {
    StateMachine<StateType>::OnStateChangeFunc onStateChangeFunc = [this](auto& states) {
        try {
            auto state = states.PrevState();
            auto& enableComponentsFunc = enableComponentsFuncs.at(state);
            enableComponentsFunc(*this, false);
        } catch (...) {}

        try {
            auto state = states.State();
            auto& enableComponentsFunc = enableComponentsFuncs.at(state);
            enableComponentsFunc(*this, true);
        } catch (...) {}
    };

    states.OverrideOnStateChangeFunc(onStateChangeFunc);
}

void ComponentsController::AddEnableComponentsFunc(StateType state, EnableComponentsFunc func) {
    states.Add(state);
    enableComponentsFuncs.insert_or_assign(state, func);

    GUARD(states.State() == state)
    states.ForceOnStateChange();
}

void ComponentsController::AddEnableComponentsById(
    StateType state, UnorderedSet<String> const& ids
) {
    AddEnableComponentsFunc(state, [ids](auto& controller, bool enable) {
        for (auto& component : controller.Target()->Components()) {
            GUARD_CONTINUE(ids.contains(component->id))
            component->Enable(enable);
        }
    });
}

void ComponentsController::AddEnableComponentsByName(
    StateType state, UnorderedSet<String> const& names
) {
    AddEnableComponentsFunc(state, [names](auto& controller, bool enable) {
        for (auto& component : controller.Target()->Components()) {
            GUARD_CONTINUE(names.contains(component->name))
            component->Enable(enable);
        }
    });
}
