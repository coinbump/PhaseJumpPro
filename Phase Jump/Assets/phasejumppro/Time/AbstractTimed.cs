using System.Collections;
using UnityEngine.Assertions;
using NUnit.Framework;

/*
 * RATING: 5 stars. Simple timer with Unit Tests
 * CODE REVIEW: 4.1.18
 */
namespace PJ {
	/// <summary>
	/// Any object that relies on timers to perform tasks
	/// 
	/// Supports an internal time scale for animating an object's time
	/// </summary>
	public abstract class AbstractTimed {
		public enum Type {
			RunOnce,	// Destroyed when finished
			Persistent	// Stays around
		}

		protected bool _isFinished;
		public bool isPaused { get; protected set; }

		public Type timedType;
		public float duration;
		public float timeScale = 1.0f;

		public AbstractTimed(Type type) {
			this.timedType = type;
		}

		public bool IsFinished
		{
			get
			{
				return _isFinished;
			}
			set
			{
				if (value != _isFinished) {
					_isFinished = value;

					if (_isFinished) {
						EvtFinished();
					}
				}
			}
		}

		public void Pause(bool pause)
		{
			if (pause != isPaused) {
				isPaused = pause;
			}
		}

		public float GetTimeDelta(TimeSlice time)
		{
			// Paused timer. No time events
			if (isPaused) {
				return 0;
			}

			var result = time.delta;
			result *= timeScale;
			return result;
		}

		public virtual void EvtUpdate(TimeSlice time) {}

		// OPTIONAL:
		protected virtual void EvtFinished() {}
		public virtual float GetProgress() { return 0.0f; }
		public virtual void Go() {}	// Called when added to time pool
	}

}
