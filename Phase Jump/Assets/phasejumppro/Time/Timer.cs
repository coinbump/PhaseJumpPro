using System;
using System.Collections;

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

		public Timer(Type type = Type.Persistent)
		: base(type)
		{
		}

		public Timer(float duration, Type type = Type.Persistent)
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
}
