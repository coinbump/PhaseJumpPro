using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple class
 * CODE REVIEW: 4/9/22
 */
namespace PJ
{
	/// <summary>
	/// A timer that runs for N duration, but whose progress is determined by an
	/// transform operation (usually an interpolation curve)
	/// </summary>
	public class TransformTimer : Timer
	{
		public SomeTransform<float> transform;

		public TransformTimer(SomeTransform<float> transform, float duration, SomeTimed.RunType type = SomeTimed.RunType.RunOnce)
			: base(duration, type)
		{
			this.transform = transform;
		}

		public override float Progress
		{
			get
			{
				var progress = base.Progress;
				if (null == transform)
				{
					return progress;
				}

				return transform.Transform(progress);
			}
		}
	}
}
