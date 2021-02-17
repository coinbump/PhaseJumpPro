using System;
using System.Collections;
using System.Collections.Generic;

namespace PJ
{
	/*
	 * RATING: 5 stars. Simple + Unit tests
	 * CODE REVIEW: 12/16/18
	 */ 
	/// <summary>
	/// Emits a pulse event after N time has passed.
	/// Useful for game objects that emit things like projectiles every N seconds.
	/// </summary>
	public class PulseTimer : SomeTimed
	{
		protected int pulseIndex;
		protected float timeInPulse;

		public int PulseIndex
		{
			get
			{
				return pulseIndex;
			}
		}

		public float TimeInPulse
		{
			get
			{
				return timeInPulse;
			}
		}

		public PulseTimer(SomeTimed.Type type) : base(type)
		{

		}

		public struct Pulse
		{
			public float time;    // Wait until N seconds to check for pulse event
			public bool pulse;    // TRUE: pulse after the time gap has expired

			public Pulse(float time, bool pulse) : this()
			{
				this.time = time;
				this.pulse = pulse;
			}
		}

		/*
		 	Each pulse occurs after N time.

		 	Example: 0-pulse, .1-pulse, 1.0-nopulse: 2 quick events, followed by a long pause, then another.
		 */
		public List<Pulse> pulses = new List<Pulse>();

		public virtual void EvtPulse(int pulseIndex)
		{
		}

		public override void EvtUpdate(TimeSlice time)
		{
			if (pulses.Count == 0) { return; }
			pulseIndex %= pulses.Count;

			float delta = time.delta;

			while (delta > 0)
			{
				Pulse thisPulse = pulses[pulseIndex];
				float timeGap = thisPulse.time;

				float newTime = timeInPulse + delta;
				if (newTime >= timeGap)
				{
					if (thisPulse.pulse)
					{
						EvtPulse(pulseIndex);
					}

					pulseIndex++;
					pulseIndex %= pulses.Count;

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

		public void Reset()
		{
			pulseIndex = 0;
			timeInPulse = 0;
		}
	}
}
