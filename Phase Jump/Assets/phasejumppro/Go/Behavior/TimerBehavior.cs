using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests (with Behavior)
 * CODE REVIEW: 4.10.18
 */
namespace PJ {

	/// <summary>
	/// Behavior runs for N seconds
	/// Example: entity searches for food for N seconds, then gives up
 	/// </summary>
	public class TimerBehavior : Behavior
	{
		Timer timer;

		public float Duration {
			get {
				return timer.duration;
			}
			set {
				timer.duration = value;
			}
		}

		public TimerBehavior() {
		}

		public TimerBehavior(float duration)
		{
			timer = new Timer(duration, AbstractTimed.Type.Persistent);
		}

		public override void EvtUpdate(TimeSlice time)
		{
			base.EvtUpdate(time);

			if (IsRunning() && null != timer && !timer.IsFinished)
			{
				timer.EvtUpdate(time);
				if (timer.IsFinished)
				{
					state.State = State.FinishedSuccess;
				}
			}
		}

		protected override void _Run()
		{
			timer.Reset();
			if (timer.duration > 0) {
				state.State = State.RunningNode;
			}
			else {
				state.State = State.FinishedSuccess;
			}
		}
	}
}
