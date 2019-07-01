using System;
using System.Collections;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Timer
	{
		[Test]
		public void UnitTests()
		{
			var timer = new Timer(1.0f, PJ.AbstractTimed.Type.Persistent);
			timer.EvtUpdate(new TimeSlice(.3f));
			Assert.AreEqual(.3f, timer.TimerState);
			Assert.AreEqual(.3f, timer.GetProgress());
			Assert.IsFalse(timer.IsFinished);
			timer.EvtUpdate(new TimeSlice(.7f));
			Assert.AreEqual(1.0f, timer.TimerState);
			Assert.AreEqual(1.0f, timer.GetProgress());
			Assert.IsTrue(timer.IsFinished);

			timer.SetProgress(.5f);
			Assert.IsFalse(timer.IsFinished);
		}
	}
}
