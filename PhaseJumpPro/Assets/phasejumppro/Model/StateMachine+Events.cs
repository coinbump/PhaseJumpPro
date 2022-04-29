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
	}
}

