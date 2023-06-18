using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 5 stars
 * Tested and works
 * CODE REVIEW: 12/26/22
 */
namespace PJ
{
    /// <summary>
    /// Animates the material's render color across the range of hues
    /// </summary>
    public class AnimateHueEffect : SomeEffect
    {
        /// <summary>
        /// Time for a full cyle across all hues
        /// </summary>
        public float cycleTime = 1.0f;

        [Range(0, 1.0f)]
        public float hue = 1.0f;

        [Range(0, 1.0f)]
        public float saturation = 1.0f;

        [Range(0, 1.0f)]
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
            var hueColor = Color.HSVToRGB(hue, saturation, value);
            MultiRenderer.Color = hueColor;
        }
    }
}
