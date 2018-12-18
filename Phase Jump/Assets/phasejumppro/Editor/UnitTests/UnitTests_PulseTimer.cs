using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_PulseTimer
	{
		protected class PulseTimer : PJ.PulseTimer
		{
			public int numPulses;

			public PulseTimer() : base(AbstractTimed.Type.Persistent) { }

			// OVERRIDE:
			public override void EvtPulse(int pulseIndex)
			{
				numPulses++;
			}
		}

		[Test]
		public void UnitTests()
		{
			PulseTimer t = new PulseTimer();
			t.pulses.Add(new PulseTimer.Pulse(0.0f, true));
			t.pulses.Add(new PulseTimer.Pulse(.1f, true));
			t.pulses.Add(new PulseTimer.Pulse(1.0f, false));

			TimeSlice time = new TimeSlice(0)
			{
				delta = 0
			};
			t.EvtUpdate(time);
			Assert.AreEqual(0, t.numPulses);

			time.delta = .05f;
			t.EvtUpdate(time);
			Assert.AreEqual(1, t.numPulses);
			Assert.AreEqual(1, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse);

			t.EvtUpdate(time);
			Assert.AreEqual(2, t.numPulses);
			Assert.AreEqual(2, t.PulseIndex);
			Assert.AreEqual(0, t.TimeInPulse);

			t.EvtUpdate(time);
			Assert.AreEqual(2, t.numPulses);
			Assert.AreEqual(2, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse);

			time.delta = 1.0f;
			t.EvtUpdate(time);
			Assert.AreEqual(3, t.numPulses);
			Assert.AreEqual(1, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse, .0001f);
		}
	}
}
