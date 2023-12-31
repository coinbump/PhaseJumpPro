using Godot;
using System.Collections;

/*
 * RATING: 5 stars
 * Tested and works
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    /// <summary>
    /// Flashes N times with the same duration for off vs on time
    /// </summary>
	public partial class FlashEffect : SomeEffect
    {
        [Export]
        public int flashCount = 3;

        [Export]
        public float flashDuration = .1f;

        [Export]
        public Color flashColor = Colors.Red;

        protected FlashTimer flashTimer;
        protected Color defaultColor = Colors.White;

        public void Reset()
        {
            flashTimer = new FlashTimer(flashCount, flashDuration);
        }

        protected override void Awake()
        {
            base.Awake();

            defaultColor = RendererTool.Color;
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

            flashTimer.OnUpdate(time);

            if (flashTimer.IsFinished)
            {
                flashTimer = null;
                RendererTool.Color = defaultColor;
            }
            else
            {
                RendererTool.Color = flashTimer.IsFlashOn ? flashColor : defaultColor;
            }
        }
    }
}
