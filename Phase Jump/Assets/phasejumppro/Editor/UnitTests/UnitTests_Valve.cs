using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	class UnitTests_Valve
	{
		private class TestValve : Valve
		{
			public TestValve()
			{
				turnOnTimer = new InterpolateTimer(new InterpolateLinear(), 1.0f, AbstractTimed.Type.Persistent);
				turnOffTimer = new InterpolateTimer(new InterpolateLinear(), 1.0f, AbstractTimed.Type.Persistent);
				TimeOn = 1.0f;
				TimeOff = 1.0f;
			}
		}

		[Test]
		public void UnitTests()
		{
			var test = new TestValve();
			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.Invalid, test.GetState());
			test.TurnOff(InputEffect.Immediate);
			Assert.AreEqual(0, test.ValveState);

			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.TurnOn, test.GetState());

			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(.5f, test.ValveState);
			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(Valve.State.On, test.GetState());

			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.TurnOff, test.GetState());

			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(.5f, test.ValveState);
			test.EvtUpdate(new TimeSlice(0.5f));
			Assert.AreEqual(Valve.State.Off, test.GetState());

			test.TimeOff = -1.0f;
			test.EvtUpdate(new TimeSlice(1.0f));
			Assert.AreEqual(Valve.State.Off, test.GetState());

			test.TimeOff = 1.0f;
			test.TurnOn(InputEffect.OverTime);
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(Valve.State.TurnOn, test.GetState());
			test.EvtUpdate(new TimeSlice(.5f));
			Assert.AreEqual(Valve.State.On, test.GetState());

			test.TurnOff(InputEffect.Immediate);
			Assert.AreEqual(Valve.State.Off, test.GetState());
			Assert.AreEqual(0, test.ValveState);
		}
	}
}
