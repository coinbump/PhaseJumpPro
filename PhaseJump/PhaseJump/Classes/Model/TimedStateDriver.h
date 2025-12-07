#pragma once

#include "List.h"
#include "StateMachine.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 9/26/24
 */
namespace PJ {
    /// Drives the states in a state machine by giving each state a duration
    template <class StateType>
    class TimedStateDriver : public SomeUpdatable {
    protected:
        /// Time in the current state since the last state transition
        float timeInState = 0;

        /// Time to spend in current state before event
        float stateDuration = 0;

        /// Countdown timer for state duration (starts from duration, counts to
        /// zero)
        float stateCountdown = 0;

    public:
        using Base = Updatable;
        using This = TimedStateDriver<StateType>;
        using Target = StateMachine<StateType>;
        using OnStateFinishFunc = std::function<void(This&)>;

        Target& target;
        OnStateFinishFunc onStateFinishFunc;
        float defaultDuration{};
        Updatable updatable;

        TimedStateDriver(Target& target) :
            target(target) {

            target.OverrideOnStateChangeFunc([this](auto& target) {
                timeInState = 0;

                SetStateDuration(defaultDuration);
            });

            updatable.onUpdateFunc = [this](auto& updatable, auto time) {
                if (stateCountdown > 0) {
                    stateCountdown -= time.delta;
                    if (stateCountdown <= 0) {
                        if (onStateFinishFunc) {
                            onStateFinishFunc(*this);
                        }
                    }
                }
                timeInState += time.delta;

                return FinishType::Continue;
            };
        }

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

        float StateProgress() const {
            GUARDR(stateDuration > 0, 0)

            float result = 1.0f - (stateCountdown / stateDuration);
            result = std::clamp(result, 0.0f, 1.0f);
            return result;
        }

        // MARK: SomeUpdatable

        FinishType OnUpdate(TimeSlice time) override {
            return updatable.OnUpdate(time);
        }

        bool IsFinished() const override {
            return updatable.IsFinished();
        }
    };
} // namespace PJ
