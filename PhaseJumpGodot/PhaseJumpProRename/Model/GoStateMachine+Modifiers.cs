using System;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/20/22
 * PORTED TO: C++
 */
namespace PJ
{
    /// <summary>
    /// State machine with state timers, to allow a state to last for N seconds
    /// </summary>
    public partial class GoStateMachine<T>
    {
        /// <summary>
        /// Modifiers can be added to the state machine to create custom behavior
        /// </summary>
        public abstract class SomeModifier : SomeRunner
        {
            public WeakReference<GoStateMachine<T>> owner;

            public SomeModifier(GoStateMachine<T> owner, SomeRunner.RunType runType) : base(runType)
            {
                this.owner = new WeakReference<GoStateMachine<T>>(owner);
            }

            public override void OnUpdate(TimeSlice time)
            {
            }
        }

        /// <summary>
        /// Do something for N seconds
        /// </summary>
        public abstract class SomeTimedModifier : SomeModifier
        {
            public float duration;

            protected float timer = 0;

            public SomeTimedModifier(float duration, GoStateMachine<T> owner, SomeRunner.RunType runType) : base(owner, runType)
            {
                this.duration = duration;
            }

            public override void OnUpdate(TimeSlice time)
            {
                if (IsFinished) { return; }
                if (timer < duration)
                {
                    timer += time.delta;

                    if (timer >= duration)
                    {
                        IsFinished = true;
                    }
                }
            }

            public override void Reset()
            {
                base.Reset();

                timer = 0;
            }
        }

        /// <summary>
        /// Run the state for N seconds, then switch to a new state
        /// </summary>
        public class TimedThenStateModifier : SomeTimedModifier
        {
            public T nextState;

            public TimedThenStateModifier(float duration, T nextState, GoStateMachine<T> owner, SomeRunner.RunType runType) : base(duration, owner, runType)
            {
                this.nextState = nextState;
            }

            protected override void OnFinish()
            {
                base.OnFinish();

                if (owner.TryGetTarget(out GoStateMachine<T> stateMachine))
                {
                    stateMachine.State = nextState;
                }
            }
        }

        /// <summary>
        /// Run the state for N seconds, then send an input for a transition
        /// </summary>
        public class TimedThenInputModifier : SomeTimedModifier
        {
            public string input;

            public TimedThenInputModifier(float duration, string input, GoStateMachine<T> owner, SomeRunner.RunType runType) : base(duration, owner, runType)
            {
                this.input = input;
            }

            protected override void OnFinish()
            {
                base.OnFinish();

                if (owner.TryGetTarget(out GoStateMachine<T> stateMachine))
                {
                    stateMachine.OnInput(input);
                }
            }
        }
    }
}
