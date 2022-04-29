using System;
using System.Collections.Generic;
using UnityEngine;

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
	public partial class StateMachine<T>
	{
		/// <summary>
		/// Modifiers can be added to the state machine to create custom behavior
		/// </summary>
		public abstract class SomeModifier
		{
			public WeakReference<StateMachine<T>> owner;
			public SomeTimed.RunType runType;
			protected bool isFinished;

			public bool IsFinished => isFinished;

			public SomeModifier(StateMachine<T> owner, SomeTimed.RunType runType)
			{
				this.owner = new WeakReference<StateMachine<T>>(owner);
				this.runType = runType;
			}

			public abstract void OnUpdate(TimeSlice time);
			public abstract void Reset();
		}

		/// <summary>
		/// Do something for N seconds
		/// </summary>
		public abstract class SomeTimedModifier : SomeModifier
		{
			public float duration;

			protected float timer = 0;

			public SomeTimedModifier(float duration, StateMachine<T> owner, SomeTimed.RunType runType) : base(owner, runType)
			{
				this.duration = duration;
			}

			public override void OnUpdate(TimeSlice time)
			{
				if (isFinished) { return; }
				if (timer < duration)
				{
					timer += time.delta;

					if (timer >= duration)
					{
						isFinished = true;
						OnFinish();
					}
				}
			}

			public override void Reset()
			{
				timer = 0;
				isFinished = false;
			}

			protected virtual void OnFinish() { }
		}

		/// <summary>
		/// Run the state for N seconds, then switch to a new state
		/// </summary>
		public class TimedThenStateModifier : SomeTimedModifier
		{
			public T nextState;

			public TimedThenStateModifier(float duration, T nextState, StateMachine<T> owner, SomeTimed.RunType runType) : base(duration, owner, runType)
			{
				this.nextState = nextState;
			}

			protected override void OnFinish()
			{
				base.OnFinish();

				if (owner.TryGetTarget(out StateMachine<T> stateMachine))
				{
					stateMachine.State = nextState;
				}
			}
		}

		/// <summary>
		/// Run teh state for N seconds, then send an input for a transition
		/// </summary>
		public class TimedThenInputModifier : SomeTimedModifier
		{
			public string input;

			public TimedThenInputModifier(float duration, string input, StateMachine<T> owner, SomeTimed.RunType runType) : base(duration, owner, runType)
			{
				this.input = input;
			}

			protected override void OnFinish()
			{
				base.OnFinish();

				if (owner.TryGetTarget(out StateMachine<T> stateMachine))
				{
					stateMachine.OnInput(input);
				}
			}
		}
	}
}
