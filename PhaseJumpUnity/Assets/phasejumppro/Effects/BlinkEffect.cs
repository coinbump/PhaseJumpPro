using UnityEngine;
using System;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// Blinks the object visible and invisible
    /// </summary>
    public class BlinkEffect : SomeEffect
    {
        protected class BlinkTimer : PulseTimer
        {
            WeakReference<BlinkEffect> owner;

            public BlinkTimer(BlinkEffect owner)
            {
                this.owner = new WeakReference<BlinkEffect>(owner);

                steps.Add(new Step(owner.blinkOffTime, true));
                steps.Add(new Step(owner.blinkOnTime, false));
            }

            protected override void OnPulse(Step step)
            {
                if (owner.TryGetTarget(out BlinkEffect animation))
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
        protected bool isBlinkOn = false;

        BlinkEffect()
        {
            blinkTimer = new BlinkTimer(this);
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }
            blinkTimer.OnUpdate(time);
        }

        public void OnPulse(bool isBlinkOn)
        {
            this.isBlinkOn = isBlinkOn;

            UpdateEffectProperties();
            if (isBlinkOn)
            {
                blinkCount++;

                if (maxBlinks > 0 && blinkCount >= maxBlinks)
                {
                    blinkTimer = null;
                }
            }
        }

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            MultiRenderer.Enabled = isBlinkOn || !IsOn;
        }
    }
}
