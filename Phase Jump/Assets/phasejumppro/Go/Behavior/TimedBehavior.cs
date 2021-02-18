using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars. Simple design pattern with Unit Tests (with Behavior)
 * CODE REVIEW: 2.17.21
 */
namespace PJ {

	/// <summary>
	/// Behavior runs for N seconds
	/// Example: entity searches for food for N seconds, then gives up
 	/// </summary>
	public class TimedBehavior : Behavior
	{
		Timer timer;

		public float Duration {
			get {
				return timer != null ? timer.duration : 0;
			}
			set {
                if (timer == null) { return; }
                timer.duration = value;
			}
		}

		public TimedBehavior() {
			timer = new Timer(0.0f, SomeTimed.Type.Persistent);
		}

		public TimedBehavior(float duration)
		{
			timer = new Timer(duration, SomeTimed.Type.Persistent);
		}

		public override void EvtUpdate(TimeSlice time)
		{
			base.EvtUpdate(time);

			if (IsRunning() && null != timer && !timer.IsFinished)
			{
				timer.EvtUpdate(time);
				if (timer.IsFinished)
				{
					state.State = State.Success;
				}
			}
		}

		protected override State Evaluate()
		{
			if (null == timer) { return State.Success; }

			timer.Reset();
			if (timer.duration > 0) {
				return State.Running;
			}
			else {
				return State.Success;
			}
		}
	}
}
