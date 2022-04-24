using System;
using System.Collections;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple timer with Unit Tests
 * CODE REVIEW: 4/14/22
 */
namespace PJ {
	/// <summary>
    /// Keeps track of time for N seconds duration, and then triggers OnFinish
    /// </summary>
	public class Timer : SomeTimed
	{
		/// <summary>
        /// Optional identifier for hashing, debug logs
        /// </summary>
		public string identifier;

		protected float timerState;  // Time state of the timer
		
		public float TimerState {
			get => timerState;
			set {
				timerState = Mathf.Min(duration, Mathf.Max(0, value));
				IsFinished = timerState >= duration;
			}
		}

		public override float Progress
		{
			get
			{
				if (duration <= 0) { return 0; }
				return Math.Min(1.0f, TimerState / duration);
			}
		}

		public Timer(float duration, RunType runType)
		: base(duration, runType)
		{
		}

		public override void Reset()
		{
			base.Reset();

			TimerState = 0;
		}

		/// <summary>
        /// Method, because overriden property is get only
        /// </summary>
        /// <param name="progress"></param>
		public void SetProgress(float progress) {
			progress = Math.Min(1.0f, Math.Max(0, progress));
			TimerState = progress * duration;
		}

		public override void OnUpdate(TimeSlice time)
		{
			if (IsFinished || duration <= 0) { return; }

			var delta = TimeDeltaFor(time);
			//Debug.Log(identifier + ": OnUpdate delta: " + delta.ToString());
			if (delta <= 0) { return; }

			TimerState += delta;
		}
	}
}
