using System;
using System.Collections;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple timer with Unit Tests
 * CODE REVIEW: 4.1.17
 */
namespace PJ {
	public class Timer : AbstractTimed
	{
		public float timerState { get; protected set; }   // Time state of the timer

		public Timer(AbstractTimed.Type type)
		: base(type)
		{
		}

		public Timer(AbstractTimed.Type type, float duration)
			: base(type)
		{
			this.duration = duration;
		}

		public void Reset()
		{
			timerState = 0;
			IsFinished = false;
			Pause(false);
		}

		public override float GetProgress()
		{
			return Math.Min(1.0f, timerState / duration);
		}

		public override void EvtUpdate(TimeSlice time)
		{
			if (IsFinished || duration <= 0) { return; }

			var delta = GetTimeDelta(time);
			if (delta <= 0) { return; }

			timerState += delta;
			timerState = Math.Min(duration, timerState);

			if (timerState >= duration)
			{
				IsFinished = true;
			}
		}
	}

	public class Timer_UnitTests {
		
		[Test]
		public void UnitTests()
		{
			var timer = new Timer(PJ.AbstractTimed.Type.Persistent, 1.0f);
			timer.EvtUpdate(new TimeSlice(.3f));
			Assert.AreEqual(.3f, timer.timerState, .001f);
			Assert.AreEqual(.3f, timer.GetProgress(), .001f);
			Assert.AreEqual(false, timer.IsFinished);
			timer.EvtUpdate(new TimeSlice(.7f));
			Assert.AreEqual(1.0f, timer.timerState, .001f);
			Assert.AreEqual(1.0f, timer.GetProgress(), .001f);
			Assert.AreEqual(true, timer.IsFinished);
		}
	}
}
