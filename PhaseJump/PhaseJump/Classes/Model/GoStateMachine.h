#ifndef PJGOSTATEMACHINE_H
#define PJGOSTATEMACHINE_H

#include "List.h"
#include "StateMachine.h"
#include "SomeRunner.h"
#include <algorithm>

/*
 RATING: 5 stars
 Has unit tests
 CODE REVIEW: 12/19/22
 */
namespace PJ
{
    template <class T>
    class EventStateFinish : public Event
    {
    public:
        using Base = Event;

        T state;

        EventStateFinish(T state, std::weak_ptr<PJ::Base> sentFrom) : Base(String(""), sentFrom), state(state)
        {
        }
    };

    /// <summary>
    /// State machine with state timers, to allow a state to last for N seconds
    /// </summary>
    template <class T>
    class GoStateMachine : public StateMachine<T>
    {
    public:
        /// <summary>
        /// Modifiers can be added to the state machine to create custom behavior
        /// </summary>
        class SomeModifier : public SomeRunner
        {
        protected:
            std::weak_ptr<GoStateMachine<T>> owner;

        public:
            SomeModifier(std::weak_ptr<GoStateMachine<T>> owner, SomeRunner::RunType runType) : SomeRunner(runType), owner(owner)
            {
            }
        };

    protected:
        /// <summary>
        /// Time in the current state since the last state transition
        /// </summary>
        float timeInState = 0;

        /// <summary>
        /// Time to spend in current state before event
        /// </summary>
        float stateDuration = 0;

        /// <summary>
        /// Countdown timer for state duration (starts from duration, counts to zero)
        /// </summary>
        float stateCountdown = 0;

    public:
        using Base = StateMachine<T>;

        /// <summary>
        /// Modifiers to modify the standard state behavior
        /// </summary>
        List<std::shared_ptr<SomeModifier>> modifiers;

        float TimeInState() const { return timeInState; }

        GoStateMachine()
        {
        }

        void SetStateDuration(float duration)
        {
            stateDuration = duration;
            stateCountdown = duration;
        }

        void ResetStateTimer()
        {
            stateCountdown = stateDuration;
        }

        void CancelStateTimer()
        {
            stateCountdown = 0; // Keep duration if we need to reset the timer later.
        }

    protected:
        /// <summary>
        /// Sets the state value without broadcasting.
        /// </summary>
        void SetStateInternal(T newState) override
        {
            Base::SetStateInternal(newState);
            CancelStateTimer(); // State duration is no longer valid for new state.
        }

    public:
        float StateProgress() const
        {
            if (stateDuration <= 0)
            {
                return 0;
            }

            float result = 1.0f - (stateCountdown / stateDuration);

            result = std::max(0.0f, std::min(1.0f, result));
            return result;
        }

    protected:
        /// <summary>
        /// Respond to state change
        /// </summary>
        void OnStateChange(T newState) override
        {
            timeInState = 0;
            Base::OnStateChange(newState);
        }

        virtual void OnStateFinish()
        {
            this->broadcaster->Broadcast(std::make_shared<EventStateFinish<T>>(this->state, this->shared_from_this()));
        }

    public:
        void OnUpdate(TimeSlice time) override
        {
            Base::OnUpdate(time);

            auto iterModifiers = modifiers;
            for (auto modifier : iterModifiers)
            {
                if (modifier->IsFinished()) {
                    modifiers.Remove(modifier);
                    continue;
                }
                modifier->OnUpdate(time);

                if (!modifier->IsFinished()) { continue; }
                modifiers.Remove(modifier);
            }

            if (stateCountdown > 0)
            {
                stateCountdown -= time.delta;
                if (stateCountdown <= 0)
                {
                    OnStateFinish();
                }
            }
            timeInState += time.delta;
        }
    };
}

#endif
