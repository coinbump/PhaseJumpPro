#pragma once

#include "CharacterController.h"
#include "Timeline.h"

// /23
namespace PJ {
    /// Tool to quickly build a character controller
    class QuickCharacter {
    public:
        using This = QuickCharacter;
        using Controller = CharacterController;
        using StateType = String;

        using BuildTimelineFunc = std::function<UP<Timeline>()>;

    protected:
        Controller& controller;
        VectorList<StateType> states;

    public:
        QuickCharacter(Controller& controller);

        /// Adds a new state to the character
        This& State(StateType state);

        /// Adds a state transition from the current state to another state
        This& TransitionTo(String input, StateType toState);

        /// Sends a state input after N seconds
        This& WaitInput(float delay, String input);

        /// When a specific signal is received, send this input to the state machine
        This& OnSignalInput(String id, String input);
    };
} // namespace PJ
