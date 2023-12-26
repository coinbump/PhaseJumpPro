using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// Flashes N times with the same duration for off vs on time
    /// </summary>
	public class FlashEffect : SomeEffect
    {
        public int flashCount = 3;
        public float flashDuration = .1f;
        public Color32 flashColor = Color.red;

        protected FlashTimer flashTimer;
        protected Color32 defaultColor = Color.white;

        public void Reset()
        {
            flashTimer = new FlashTimer(flashCount, flashDuration);
        }

        protected override void Awake()
        {
            base.Awake();

            defaultColor = MultiRenderer.Color;
            Reset();
        }

        protected override void OnSwitchChange()
        {
            base.OnSwitchChange();
            Reset();
        }

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }
            if (null == flashTimer) { return; }

            flashTimer.OnUpdate(new TimeSlice(Time.deltaTime));

            if (flashTimer.IsFinished)
            {
                flashTimer = null;
                MultiRenderer.Color = defaultColor;
            }
            else
            {
                MultiRenderer.Color = flashTimer.IsFlashOn ? flashColor : defaultColor;
            }
        }
    }
}
