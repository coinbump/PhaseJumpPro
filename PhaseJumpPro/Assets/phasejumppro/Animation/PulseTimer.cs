using System;
using System.Collections;
using System.Collections.Generic;

/*
 * RATING: 5 stars
 * Has unit tests
 * CODE REVIEW: 4/16/22
 */
namespace PJ
{
	/// <summary>
	/// Emits a pulse event after N time has passed.
	/// Useful for game objects that emit things like projectiles every N seconds.
	/// </summary>
	public class PulseTimer : SomeTimed
	{
		protected int pulseIndex;
		protected float timeInPulse;

		public int PulseIndex => pulseIndex;
		public float TimeInPulse => timeInPulse;

		public override float Progress => 0.0f;

		public PulseTimer() : base(0, RunType.KeepRunning)
		{
		}

		public struct Step
		{
			public float time;		// Wait until N seconds to check for pulse event
			public bool isPulse;    // TRUE: pulse after the time gap has expired (need no-pulse events for loops, to create delays)

			public Step(float time, bool isPulse) : this()
			{
				this.time = time;
				this.isPulse = isPulse;
			}
		}

		/*
		 	Each pulse occurs after N time.

		 	Example: 0-pulse, .1-pulse, 1.0-nopulse: 2 quick events, followed by a long pause, then another.
		 */
		public List<Step> steps = new List<Step>();

		protected virtual void OnPulse(Step pulse)
		{
		}

		public override void OnUpdate(TimeSlice time)
		{
			if (steps.Count == 0) { return; }
			pulseIndex %= steps.Count;

			float delta = time.delta;

			while (delta > 0)
			{
				Step thisPulse = steps[pulseIndex];
				float timeGap = thisPulse.time;

				float newTime = timeInPulse + delta;
				if (newTime >= timeGap)
				{
					OnPulse(thisPulse);

					pulseIndex++;
					pulseIndex %= steps.Count;

					float timeDelta = timeGap - timeInPulse;
					delta -= timeDelta;
					timeInPulse = 0;
				}
				else
				{
					timeInPulse += delta;
					break;
				}
			}
		}

		public override void Reset()
		{
			base.Reset();

			pulseIndex = 0;
			timeInPulse = 0;
		}
	}
}
