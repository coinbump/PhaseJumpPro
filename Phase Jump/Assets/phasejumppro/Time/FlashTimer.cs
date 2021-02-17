using UnityEngine;
using System.Collections;

namespace PJ
{
	/// <summary>
	/// Flashes on and off N times
	/// Used for game objects that flash (usually after they receive damage)
	/// 
	/// SEE ALSO: PulseTimer (more generalized)
	/// </summary>
	public class FlashTimer : SomeTimed
	{
		public int flashCount = 3;

		protected bool isFlashOn;

		protected int curFlashCount = 0;
		protected Timer timer = new Timer(SomeTimed.Type.Persistent);

		public bool IsFlashOn
		{
			get
			{
				return isFlashOn;
			}
		}

		public FlashTimer(int flashCount, float flashDuration) : base(SomeTimed.Type.Persistent)
		{
			this.flashCount = flashCount;
			timer.duration = flashDuration;
		}

		public void Reset()
		{
			curFlashCount = 0;
			isFlashOn = false;
			timer.Reset();
		}

		public override void EvtUpdate(TimeSlice time)
		{
			base.EvtUpdate(time);

			if (!IsRunning)
			{
				return;
			}

			if (timer.IsFinished)
			{
				return;
			}

			timer.EvtUpdate(time);
			if (timer.IsFinished)
			{
				EvtTimerFinished();
			}
		}

		protected virtual void EvtFlash() { }

		protected void EvtTimerFinished()
		{
			isFlashOn = !isFlashOn;
			EvtFlash();

			// Increment on flash off so we don't get stuck with it on
			if (!isFlashOn)
			{
				curFlashCount += 1;
				if (flashCount > 0 && curFlashCount >= flashCount)
				{
					Pause(true);	// Pause when finished.
					return;
				}
			}
			timer.Reset();
		}
	}
}