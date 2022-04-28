
/*
 * RATING: 5 stars. Simple classes.
 * CODE REVIEW: 4.9.22
 */
namespace PJ
{
	/// <summary>
	/// Event: goal has been completed
	/// </summary>
	public class EventGoalComplete : Event
	{
		public SomeGoal goal;

		public EventGoalComplete(SomeGoal goal) {
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
