using UnityEngine;

/*
RATING: 5 STARS
Tested and works
CODE REVIEW: 12/26/22
*/
namespace PJ
{
    public class AlphaEffect : SomeAnimatedEffect
    {
        public float offAlpha = 1.0f;
        public float onAlpha = 0.5f;

        public override void UpdateEffectProperties()
        {
            base.UpdateEffectProperties();

            // Bugfix: Changing the color while in the editor causes prefabs to lose their material
            if (Application.isEditor && !Application.isPlaying) { return; }

            var valveState = valve.ValveState;
            Color color = MultiRenderer.Color;
            color.a = new FloatValueInterpolator().ValueAt(offAlpha, onAlpha, valveState);
            MultiRenderer.Color = color;
        }
    }
}
