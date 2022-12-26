using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/19/22
 */
namespace PJ
{
    /// <summary>
    /// State machine with state timers
    /// </summary>
    public partial class GoStateMachine<T> : StateMachine<T>
    {
        /// <summary>
        /// Modifiers to modify the standard state behavior
        /// </summary>
        public List<SomeModifier> modifiers = new();

        /// <summary>
        /// Time in the current state since the last state transition
        /// Important: You must call OnUpdate for this to work
        /// </summary>
        public float timeInState { get; protected set; }

        // OPTIONAL:
        /// <summary>
        /// Time to spend in current state before notification
        /// </summary>
        protected float stateDuration;

        /// <summary>
        /// Countdown timer for state duration (starts from duration, counts to zero)
        /// </summary>
        protected float stateCountdown;

        public GoStateMachine() : base()
        {
        }

        public GoStateMachine(SomeListener listener) : base(listener)
        {
        }

        public void SetStateDuration(float duration)
        {
            stateDuration = duration;
            stateCountdown = duration;
        }

        public void ResetStateTimer()
        {
            stateCountdown = stateDuration;
        }

        public void CancelStateTimer()
        {
            stateCountdown = 0; // Keep duration if we need to reset the timer later.
        }

        /// <summary>
        /// Sets the state value without broadcasting.
        /// </summary>
        protected override void SetStateInternal(T newState)
        {
            base.SetStateInternal(newState);
            CancelStateTimer(); // State duration is no longer valid for new state.
        }

        public float StateProgress
        {
            get
            {
                if (stateDuration <= 0)
                {
                    return 0;
                }

                float result = 1.0f - (stateCountdown / stateDuration);

                result = Math.Max(0, Math.Min(1.0f, result));
                return result;
            }
        }

        /// <summary>
        /// Respond to state change
        /// </summary>
        protected override void OnStateChange(T newState)
        {
            timeInState = 0;
            base.OnStateChange(newState);
        }

        protected virtual void OnStateFinish()
        {
            broadcaster.Broadcast(new EventStateFinish<T>(state, this));
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            var iterModifiers = new HashSet<SomeModifier>(modifiers);
            foreach (SomeModifier modifier in iterModifiers)
            {
                if (modifier.IsFinished)
                {
                    modifiers.Remove(modifier);
                    continue;
                }
                modifier.OnUpdate(time);

                if (!modifier.IsFinished) { continue; }
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
    }
}
