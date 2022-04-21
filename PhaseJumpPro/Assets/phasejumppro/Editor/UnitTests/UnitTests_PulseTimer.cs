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

			public PulseTimer() : base() { }

			// OVERRIDE:
			protected override void OnPulse(Step pulse)
			{
				if (pulse.isPulse)
				{
					numPulses++;
				}
			}
		}

		[Test]
		public void UnitTests()
		{
			PulseTimer t = new PulseTimer();
			t.steps.Add(new PulseTimer.Step(0.0f, true));
			t.steps.Add(new PulseTimer.Step(.1f, true));
			t.steps.Add(new PulseTimer.Step(1.0f, false));

			TimeSlice time = new TimeSlice(0)
			{
				delta = 0
			};
			t.OnUpdate(time);
			Assert.AreEqual(0, t.numPulses);

			time.delta = .05f;
			t.OnUpdate(time);
			Assert.AreEqual(1, t.numPulses);
			Assert.AreEqual(1, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse);

			t.OnUpdate(time);
			Assert.AreEqual(2, t.numPulses);
			Assert.AreEqual(2, t.PulseIndex);
			Assert.AreEqual(0, t.TimeInPulse);

			t.OnUpdate(time);
			Assert.AreEqual(2, t.numPulses);
			Assert.AreEqual(2, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse);

			time.delta = 1.0f;
			t.OnUpdate(time);
			Assert.AreEqual(3, t.numPulses);
			Assert.AreEqual(1, t.PulseIndex);
			Assert.AreEqual(.05f, t.TimeInPulse, .0001f);
		}
	}
}
