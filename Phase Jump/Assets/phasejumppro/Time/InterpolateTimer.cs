using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars. Simple class, no Unit Tests needed
 * CODE REVIEW: 4.8.18
 */
namespace PJ
{
	/// <summary>
	/// A timer that runs for N duration, but whose progress is determined by an
	/// interpolate curve
	/// </summary>
	public class InterpolateTimer : Timer
	{
		public Interpolate interpolate;

		public InterpolateTimer(Interpolate interpolate, float duration, AbstractTimed.Type type)
			: base(type, duration)
		{
			this.interpolate = interpolate;
		}

		public override float GetProgress()
		{
			var progress = base.GetProgress();
			if (interpolate == null) {
				return progress;
			}

			return interpolate.Transform(progress);
		}

	}

}
