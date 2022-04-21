using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/18/22
 */
namespace PJ
{
	/// <summary>
	/// Fades in over N duration time.
	/// </summary>
	public class FadeAnimation : SomeAnimation
	{
		public float duration = 1.0f;

		[Range(0, 1.0f)]
		public float startAlpha = 0.0f;

		[Range(0, 1.0f)]
		public float endAlpha = 1.0f;

		/// <summary>
        /// If true, turn off collisions while the animation is running
        /// </summary>
		public bool disableCollisions = true;

		protected Timer timer = new Timer(1.0f, SomeTimed.RunType.RunOnce);
		protected bool wasColliderEnabled = true;

		// FUTURE: support custom interpolate if needed
		protected SomeTransform<float> interpolate = new InterpolateOutSquared();

		protected MultiCollider multiCollider;

		protected override void Awake()
		{
			base.Awake();

			timer.duration = duration;
			multiCollider = new MultiCollider(gameObject);
			multiRenderer = new MultiRenderer(gameObject);

			if (!enabled) { return;  }
			Color color = multiRenderer.Color;
			color.a = startAlpha;
			multiRenderer.Color = color;

			wasColliderEnabled = multiCollider.Enabled;
			if (disableCollisions)
			{
				multiCollider.Enabled = false;
			}
		}

		protected override void Start()
		{
			base.Start();
		}

		protected override void Update()
		{
			base.Update();

			if (timer.IsFinished) { return; }

			var timeSlice = new TimeSlice(Time.deltaTime);
			timer.OnUpdate(timeSlice);

			Color color = multiRenderer.Color;
			color.a = startAlpha + (endAlpha - startAlpha) * interpolate.Transform(timer.Progress);

			if (timer.IsFinished)
			{
				color.a = endAlpha;

				// Stop receiving update events, animation is finished
				enabled = false;

				if (disableCollisions)
                {
					multiCollider.Enabled = wasColliderEnabled;
				}
			}

			multiRenderer.Color = color;
		}
	}
}
