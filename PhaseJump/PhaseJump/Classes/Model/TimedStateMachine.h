#pragma once

#include "List.h"
#include "StateMachine.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 7/7/24
 */
namespace PJ {
    /// State machine with state timers, to allow a state to last for N seconds
    template <class StateType>
    class TimedStateMachine : public StateMachine<StateType> {
    protected:
        /// Time in the current state since the last state transition
        float timeInState = 0;

        /// Time to spend in current state before event
        float stateDuration = 0;

        /// Countdown timer for state duration (starts from duration, counts to
        /// zero)
        float stateCountdown = 0;

    public:
        using Base = StateMachine<StateType>;

        TimedStateMachine() {}

        float TimeInState() const {
            return timeInState;
        }

        void SetStateDuration(float duration) {
            stateDuration = duration;
            stateCountdown = duration;
        }

        void ResetStateTimer() {
            stateCountdown = stateDuration;
        }

        void CancelStateTimer() {
            stateCountdown = 0; // Keep duration if we need to reset the timer later.
        }

    protected:
        /// Sets the state value without broadcasting.
        void SetStateInternal(StateType newState) override {
            Base::SetStateInternal(newState);
            CancelStateTimer(); // State duration is no longer valid for new state.
        }

    public:
        float StateProgress() const {
            if (stateDuration <= 0) {
                return 0;
            }

            float result = 1.0f - (stateCountdown / stateDuration);

            result = std::max(0.0f, std::min(1.0f, result));
            return result;
        }

    protected:
        /// Respond to state change
        void OnStateChange(StateType newState) override {
            timeInState = 0;
            Base::OnStateChange(newState);
        }

        virtual void OnStateFinish() {}

    public:
        void OnUpdate(TimeSlice time) override {
            Base::OnUpdate(time);

            if (stateCountdown > 0) {
                stateCountdown -= time.delta;
                if (stateCountdown <= 0) {
                    OnStateFinish();
                }
            }
            timeInState += time.delta;
        }
    };
} // namespace PJ
