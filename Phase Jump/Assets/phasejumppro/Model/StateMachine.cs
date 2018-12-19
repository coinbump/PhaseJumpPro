using System;

/*
 * RATING: 5 stars. Core functionality with Unit Tests
 * CODE REVIEW: 3.18.18
 */
namespace PJ {

	/// <summary>
	/// Abstract state machine.
	/// </summary>
	public abstract class AbstractStateMachine {
		public abstract void AddListener(Listener listener);
		public abstract void EvtUpdate(TimeSlice time);
    }

	/// <summary>
	/// Generic state machine (typically T is an enum).
	/// </summary>
	public class GenericStateMachine<T> : AbstractStateMachine where T : struct, IConvertible
	{
		// USAGE: best practice: the T type should have a default value that represents an invalid state
		public T state { get; protected set; }
		public T prevState { get; protected set; }

		// OPTIONAL:
		protected float stateDuration;
		protected float stateReverseTimer;
		protected bool isLocked;
		public float timeInState { get; protected set; }
		public Broadcaster broadcaster = new Broadcaster();

		public override void AddListener(Listener listener)
		{
			broadcaster.AddListener(listener);
		}

		public void Lock(bool lockState)
		{
			isLocked = lockState;
		}

		public void SetStateDuration(float duration)
		{
			stateDuration = duration;
			stateReverseTimer = duration;
		}

		public void ResetStateTimer()
		{
			stateReverseTimer = stateDuration;
		}

		public void CancelStateTimer()
		{
			stateReverseTimer = 0; // Keep duration if we need to reset the timer later.
		}

		public T State
		{
			get
			{
				return state;
			}
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

				SetStateValue(newState);
				EvtStateChanged(newState);
			}
		}

		/// <summary>
		/// Sets the state value without broadcasting.
		/// </summary>
		/// <param name="newState">New state.</param>
		public void SetStateValue(T newState)
		{
			if (newState.Equals(state))
			{
				return;
			}
			prevState = state;
			state = newState;
			CancelStateTimer(); // State duration is no longer valid for new state.
		}

		public float Progress
		{
			get
			{
				if (stateDuration.Equals(0))
				{
					return 0;
				}

				float result = 1.0f - (stateReverseTimer / stateDuration);

				// Cap values to avoid weirdness in case of sudden time jumps.
				result = Math.Max(0, Math.Min(1.0f, result));
				return result;
			}
		}

		// Override to handle logic of a state transition
		public virtual bool CanTransition(T newState)
		{
			return true; // TRUE: go ahead and change states
		}

		// Override to respond to state changes.
		protected virtual void EvtStateChanged(T newState)
		{
			timeInState = 0;

			broadcaster.Broadcast(new Event(EventNames.StateChanged, new WeakReference(this)));
		}

		protected virtual void EvtStateFinished()
		{
			broadcaster.Broadcast(new Event(EventNames.StateFinished, new WeakReference(this)));
		}

		public override void EvtUpdate(TimeSlice time)
		{
			if (stateReverseTimer > 0)
			{
				stateReverseTimer -= time.delta;
				if (stateReverseTimer <= 0)
				{
					EvtStateFinished();
				}
			}
			timeInState += time.delta;
		}
	}       
}

