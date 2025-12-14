using System.Collections;
using System.Collections.Generic;
using Godot;

/*
 * RATING: 5 stars
 * Tested and works
 * GODOT CODE REVIEW: 12/30/23
 */
namespace PJ
{
    /// <summary>
    /// Animates the material's render color across the range of hues
    /// </summary>
    public partial class AnimateHueEffect : SomeEffect
    {
        /// <summary>
        /// Time for a full cyle across all hues
        /// </summary>
        [Export]
        public float cycleTime = 1.0f;

        [Export(PropertyHint.Range, "0,1.0,")]
        public float hue = 1.0f;

        [Export(PropertyHint.Range, "0,1.0,")]
        public float saturation = 1.0f;

        [Export(PropertyHint.Range, "0,1.0,")]
        public float value = 1.0f;

        public override void OnUpdate(TimeSlice time)
        {
            base.OnUpdate(time);

            if (!IsOn) { return; }

            var newHue = hue;
            newHue += time.delta / cycleTime;
            newHue %= 1.0f;

            hue = newHue;
            UpdateEffectProperties();
        }

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            if (!IsOn) { return; }
            var hueColor = Color.FromHsv(hue, saturation, value);
            RendererTool.Color = hueColor;
        }
    }
}
