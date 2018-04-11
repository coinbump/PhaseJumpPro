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
		private float _timerState;  // Time state of the timer

		public float TimerState {
			get {
				return _timerState;
			}
			set {
				_timerState = Math.Min(duration, Math.Max(0, value));
				IsFinished = _timerState >= duration;
			}
		}

		public Timer(AbstractTimed.Type type)
		: base(type)
		{
		}

		public Timer(float duration, AbstractTimed.Type type)
			: base(type)
		{
			this.duration = duration;
		}

		public void Reset()
		{
			TimerState = 0;
			IsFinished = false;
			Pause(false);
		}

		public override float GetProgress()
		{
			return Math.Min(1.0f, TimerState / duration);
		}

		public void SetProgress(float progress) {
			progress = Math.Min(1.0f, Math.Max(0, progress));
			TimerState = progress * duration;
		}

		public override void EvtUpdate(TimeSlice time)
		{
			if (IsFinished || duration <= 0) { return; }

			var delta = GetTimeDelta(time);
			if (delta <= 0) { return; }

			TimerState = TimerState + delta;
		}
	}

	public class Timer_UnitTests {
		
		[Test]
		public void UnitTests()
		{
			var timer = new Timer(1.0f, PJ.AbstractTimed.Type.Persistent);
			timer.EvtUpdate(new TimeSlice(.3f));
			Assert.AreEqual(.3f, timer.TimerState);
			Assert.AreEqual(.3f, timer.GetProgress());
			Assert.AreEqual(false, timer.IsFinished);
			timer.EvtUpdate(new TimeSlice(.7f));
			Assert.AreEqual(1.0f, timer.TimerState);
			Assert.AreEqual(1.0f, timer.GetProgress());
			Assert.AreEqual(true, timer.IsFinished);

			timer.SetProgress(.5f);
			Assert.AreEqual(false, timer.IsFinished);
		}
	}
}
