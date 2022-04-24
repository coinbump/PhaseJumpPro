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
		protected SomeTransform<float> interpolate = new InterpolateLinear();

		protected MultiCollider multiCollider;
		protected RendererAlphaAttributeAnimator attributeAnimator;

		protected override void Awake()
		{
			base.Awake();

			attributeAnimator = new RendererAlphaAttributeAnimator(gameObject);

			timer.duration = duration;
			multiCollider = new MultiCollider(gameObject);

			if (!enabled) { return;  }

			attributeAnimator.ApplyAttributeValueFor(startAlpha, endAlpha, 0);
			
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

			if (timer.IsFinished)
			{
				// Stop receiving update events, animation is finished
				enabled = false;

				if (disableCollisions)
                {
					multiCollider.Enabled = wasColliderEnabled;
				}
			}

			attributeAnimator.ApplyAttributeValueFor(startAlpha, endAlpha, interpolate.Transform(timer.Progress));
		}
	}
}
