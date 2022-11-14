using System.Collections;
using UnityEngine.Assertions;

/*
 * RATING: 5 stars
 * Has unit tests (via Timer)
 * CODE REVIEW: 4/14/22
 * 
 * PORTED TO: C++
 */
namespace PJ {
	/// <summary>
	/// Any object that relies on timers to perform tasks
	/// 
	/// Supports an internal time scale for animating an object's time
	/// </summary>
	public abstract class SomeTimed : Updatable {
		public abstract float Progress { get; }

		public enum RunType {
            // Destroy when finished
            RunOnce,

            // Repeats (example: timer that does an update every N seconds)
            KeepRunning
        }

		public float duration;
		public RunType runType;

		/// <summary>
		/// Used to speed up/slow down timer
		/// </summary>
		public float timeScale = 1.0f;

		/// <summary>
        /// If true, timer has reached the end of its duration
        /// </summary>
		protected bool isFinished;

		/// <summary>
        /// If true, timer will stop receiving time events
        /// </summary>
		protected bool isPaused;

		public SomeTimed(float duration, RunType runType) {
			this.duration = duration;
			this.runType = runType;
		}

		public bool IsFinished
		{
			get => isFinished;
			set
			{
				if (value != isFinished) {
					isFinished = value;

					if (isFinished) {
						OnFinish();

						switch (runType)
						{
							case RunType.KeepRunning:
								Reset();
								break;
							default:
								break;
						}

					}
				}
			}
		}

		public bool IsRunning
		{
			get => !isPaused;
			set
			{
				isPaused = !value;
			}
		}

		public void Pause(bool pause)
		{
			IsRunning = !pause;
		}

		public virtual void Reset()
        {
			isFinished = false;
			Pause(false);
		}

		public float TimeDeltaFor(TimeSlice time)
		{
			// Paused timer. No time events
			if (isPaused) {
				return 0;
			}

			var result = time.delta;
			result *= timeScale;
			return result;
		}

		public virtual void OnUpdate(TimeSlice time) {}

		protected virtual void OnFinish() { }
		public virtual void Go() {}
	}
}
