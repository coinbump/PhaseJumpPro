using System;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
	/// <summary>
	/// State machine with state timers, to allow a state to last for N seconds
	/// </summary>
	public class StateMachine<T> : SomeStateMachine, Updatable
	{
		public class EventStateChange : Event
		{
			public T prevState { get; protected set; }
			public T state { get; protected set; }

			public EventStateChange(T prevState, T state, object sentFrom) : base("", sentFrom)
			{
				this.prevState = prevState;
				this.state = state;
			}
		}

		public class EventStateFinish : Event
        {
			public T state { get; protected set; }

			public EventStateFinish(T state, object sentFrom) : base("", sentFrom)
			{
				this.state = state;
			}
        }

		public T state { get; protected set; }
		public T prevState { get; protected set; }

		/// <summary>
        /// Broadcast state change events
        /// </summary>
		public Broadcaster broadcaster { get; protected set; } = new Broadcaster();

		/// <summary>
        /// Time in the current state since the last state transition (requires OnUpdate)
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
		protected float stateCountdownTimer;

		/// <summary>
        /// If true, state transitions can't occur
        /// </summary>
		protected bool isLocked;

		public bool IsLocked
        {
			get => isLocked;
			set
            {
				isLocked = value;
            }
        }

		public StateMachine()
        {
        }

		public StateMachine(SomeListener listener)
        {
			AddListener(listener);
        }

		public void AddListener(SomeListener listener)
		{
			broadcaster.AddListener(listener);
		}

		public void SetStateDuration(float duration)
		{
			stateDuration = duration;
			stateCountdownTimer = duration;
		}

		public void ResetStateTimer()
		{
			stateCountdownTimer = stateDuration;
		}

		public void CancelStateTimer()
		{
			stateCountdownTimer = 0; // Keep duration if we need to reset the timer later.
		}

		public T State
		{
			get => state;
			set
			{
				var newState = value;
				if (newState.Equals(state))
				{
					return;
				}
				if (isLocked)
				{
					return;
				}
				if (!CanTransition(newState))
				{
					return;
				}

				SetStateInternal(newState);
				OnStateChange(newState);
			}
		}

		/// <summary>
		/// Sets the state value without broadcasting.
		/// </summary>
		/// <param name="newState">New state.</param>
		protected void SetStateInternal(T newState)
		{
			prevState = state;
			state = newState;
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

				float result = 1.0f - (stateCountdownTimer / stateDuration);

				result = Math.Max(0, Math.Min(1.0f, result));
				return result;
			}
		}

		/// <summary>
        /// Return true if we can transition to the new state
        /// </summary>
        public virtual bool CanTransition(T newState) => true;

		/// <summary>
        /// Respond to state change
        /// </summary>
		protected virtual void OnStateChange(T newState)
		{
			timeInState = 0;
			broadcaster.Broadcast(new EventStateChange(prevState, state, this));
		}

		protected virtual void OnStateFinish()
		{
			broadcaster.Broadcast(new EventStateFinish(state, this));
		}

		public virtual void OnUpdate(TimeSlice time)
		{
			if (stateCountdownTimer > 0)
			{
				stateCountdownTimer -= time.delta;
				if (stateCountdownTimer <= 0)
				{
					OnStateFinish();
				}
			}
			timeInState += time.delta;
		}
	}
}

