#pragma once

#include "TimedStateMachine.h"

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/7/23
 */
namespace PJ {
    /// Interface to owner of a ComposeTimedStateMachine
    template <class StateType>
    class SomeTimedStateMachineOwner {
    public:
        virtual ~SomeTimedStateMachineOwner() {}

        virtual void OnStateChange(TimedStateMachine<StateType>& inStateMachine) = 0;
        virtual void OnStateFinish(TimedStateMachine<StateType>& inStateMachine) = 0;
    };

    // TODO: can this be replaced with funcs?
    /// Convenience class for composing TimedStateMachine
    template <class StateType>
    class ComposeTimedStateMachine : public TimedStateMachine<StateType> {
    public:
        using Base = TimedStateMachine<StateType>;

        SomeTimedStateMachineOwner<StateType>& owner;

    public:
        ComposeTimedStateMachine(SomeTimedStateMachineOwner<StateType>& owner) :
            owner(owner) {}

        void OnStateChange(StateType newState) override {
            Base::OnStateChange(newState);

            owner.OnStateChange(*this);
        }

        void OnStateFinish() override {
            Base::OnStateFinish();

            owner.OnStateFinish(*this);
        }
    };
} // namespace PJ
