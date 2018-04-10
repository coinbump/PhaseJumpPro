using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple classes.
 * CODE REVIEW: 4.9.18
 */
namespace PJ
{
	/// <summary>
	/// Event: goal has been completed
	/// </summary>
	public class EventGoalComplete : Event
	{
		public Goal goal;

		public EventGoalComplete(Goal goal) {
			this.goal = goal;
		}
	}

	/// <summary>
	/// Event that carries a signal (see Signal-Receptor) pattern
	/// Useful for responding to categories of behavior
	/// </summary>
	public class EventSignal : Event {
		public Signal<string> signal = new Signal<string>();
	}
}
