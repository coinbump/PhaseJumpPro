using UnityEngine;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works with text, sprite
 * CODE REVIEW: 4/20/22
 */
namespace PJ
{
    /// <summary>
    /// Flashes N times with the same duration for off vs on time
    /// </summary>
	public class FlashAnimation : SomeAnimation
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

            defaultColor = multiRenderer.Color;

            Reset();
        }

        protected override void Start()
		{
            base.Start();
        }

        protected override void Update()
		{
            base.Update();

            if (null != flashTimer)
            {
                flashTimer.OnUpdate(new TimeSlice(Time.deltaTime));

                if (flashTimer.IsFinished)
                {
                    flashTimer = null;
                    multiRenderer.Color = defaultColor;
                }
                else
                {
                    multiRenderer.Color = flashTimer.IsFlashOn ? flashColor : defaultColor;
                }
            }
        }
	}
}
