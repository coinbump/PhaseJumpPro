using UnityEngine;
using System;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
	/// <summary>
    /// Blinks the object visible and invisible
    /// </summary>
	public class BlinkAnimation : SomeAnimation
	{
		protected class BlinkTimer : PulseTimer
        {
			WeakReference<BlinkAnimation> owner;

			public BlinkTimer(BlinkAnimation owner)
            {
				this.owner = new WeakReference<BlinkAnimation>(owner);

				steps.Add(new Step(owner.blinkOffTime, true));
				steps.Add(new Step(owner.blinkOnTime, false));
			}

			protected override void OnPulse(Step step)
            {
				if (owner.TryGetTarget(out BlinkAnimation animation))
                {
					animation.OnPulse(step.isPulse);
                }
            }
        }

		public float blinkOffTime = .1f;
		public float blinkOnTime = .1f;
		public int maxBlinks = 0;

		protected BlinkTimer blinkTimer;
		protected int blinkCount = 0;

		protected override void Awake()
        {
			base.Awake();

			blinkTimer = new BlinkTimer(this);
		}

		protected override void Start()
		{
			base.Start();

			multiRenderer.Enabled = false;
		}

		protected override void Update()
		{
			base.Update();

			if (null == blinkTimer) { return; }
			blinkTimer.OnUpdate(new TimeSlice(Time.deltaTime));
		}

		public void OnPulse(bool isBlinkOn)
        {
			multiRenderer.Enabled = isBlinkOn;
			if (isBlinkOn)
            {
				blinkCount++;

				if (maxBlinks > 0 && blinkCount >= maxBlinks)
                {
					blinkTimer = null;
                }
            }
        }
	}
}
