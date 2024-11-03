#include "QuickCharacter.h"

using namespace std;
using namespace PJ;

using This = QuickCharacter;

QuickCharacter::QuickCharacter(Controller& controller) :
    controller(controller) {}

This& QuickCharacter::State(StateType state) {
    controller.states.Add(state);
    states.push_back(state);

    return *this;
}

This& QuickCharacter::TransitionTo(String input, StateType toState) {
    GUARDR_LOG(!IsEmpty(states), *this, "ERROR. No from state for transition")

    auto& state = states[states.size() - 1];
    controller.states.AddTransition(state, input, toState);
    return *this;
}

This& QuickCharacter::WaitInput(float delay, String input) {
    controller.updatables.AddDelay(delay, [input, this](auto time) {
        controller.states.Input(input);
        return FinishType::Finish;
    });
    return *this;
}

This& QuickCharacter::OnSignalInput(String id, String input) {
    controller.signalFuncs[id] = [=](auto& component, auto& signal) {
        auto& controller = *(static_cast<Controller*>(&component));
        controller.states.Input(input);
    };
    return *this;
}
