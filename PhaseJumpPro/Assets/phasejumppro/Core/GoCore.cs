using System;
using System.Collections;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 12/20/22
 * PORTED TO: C++
 */
namespace PJ
{
    public interface SomeGoStateListener<StateType>
    {
        public void OnStateChange(GoStateMachine<StateType> inStateMachine);
        public void OnStateFinish(GoStateMachine<StateType> inStateMachine);
    }

    /// <summary>
    /// Provides common design patterns:
    /// - Broadcaster, for broadcast-listen events
    /// - State machine, for state changes
    /// </summary>
    public partial class GoCore<StateType> : Core, Updatable
    {
        public WeakReference<SomeGoStateListener<StateType>> owner;
        public GoStateMachine<StateType> sm = new GoStateMachine<StateType>();

        GoStateMachine<StateType> StateMachine => sm;

        public StateType State
        {
            get => sm.State;
            set
            {
                sm.State = value;
            }
        }

        public Broadcaster broadcaster = new Broadcaster();

        public GoCore()
        {
            sm.AddListener(this);
        }

        public GoCore(SomeGoStateListener<StateType> owner)
        {
            sm.AddListener(this);
            this.owner = new WeakReference<SomeGoStateListener<StateType>>(owner);
        }

        protected virtual void OnStateChange(GoStateMachine<StateType> inStateMachine)
        {
            if (null == this.owner) { return; }
            if (!this.owner.TryGetTarget(out SomeGoStateListener<StateType> owner)) { return; }
            owner.OnStateChange(inStateMachine);
        }

        protected virtual void OnStateFinish(GoStateMachine<StateType> inStateMachine)
        {
            if (null == this.owner) { return; }
            if (!this.owner.TryGetTarget(out SomeGoStateListener<StateType> owner)) { return; }
            owner.OnStateFinish(inStateMachine);
        }

        public virtual void OnUpdate(TimeSlice time)
        {
            sm.OnUpdate(time);
        }

        public bool IsFinished { get => false; }
    }

    public partial class GoCore<StateType> : SomeListener
    {
        public virtual void OnListen(Event theEvent)
        {
            if (!theEvent.sentFrom.TryGetTarget(out object sentFrom)) { return; }

            var asStateMachine = sentFrom as GoStateMachine<StateType>;
            if (null == asStateMachine) { return; }

            var stateChangeEvent = theEvent as EventStateChange<StateType>;
            if (null != stateChangeEvent)
            {
                OnStateChange(asStateMachine);
                return;
            }

            var stateFinishEvent = theEvent as EventStateFinish<StateType>;
            if (null != stateFinishEvent)
            {
                OnStateFinish(asStateMachine);
                return;
            }
        }
    }
}
